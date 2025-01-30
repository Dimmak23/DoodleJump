//* Source header
#include "level/world/dynamic/WorldDynamic.hpp"

//* SDL2: Connector
#include "SDL2_connector/Loger.hpp"

//* Game
#include "abilities/wormhole/Wormhole.hpp"
#include "graphics/image/Image.hpp"
#include "level/level/I_Stoppable.hpp"
#include "modules/collider/I_Collidable.hpp"
#include "modules/locator/Locator.hpp"
#include "modules/physics_engine/I_Mechanics.hpp"
#include "modules/physics_engine/PhysicsEngine.hpp"
#include "modules/scene/I_LocatableScene.hpp"
#include "modules/spawn/RandomSpawn.hpp"
#include "projectiles/ammo/Ammo.hpp"
#include "screen/ScreenItem.hpp"
#include "utilities/Sizes.hpp"

//* C++ std
#include <format>
#include <iostream>
#include <string>

WorldDynamic::WorldDynamic(const ScreenItem* parent_screen,		   //
						   const char* enemies_path_cut,		   //
						   const char* enemies_path_adder,		   //
						   const unsigned int enemies_quantity,	   //
						   const char* wormhole_path,			   //
						   const char* ammo_path)
	: _screen(parent_screen)
	, _enemiesPathCut(enemies_path_cut)
	, _enemiesPathAdder(enemies_path_adder)
	, _enemiesQuantity(enemies_quantity)
	, _wormholePath(wormhole_path)
	, _ammoPath(ammo_path)
{
	_thresholdY = int(float(_screen->_applicationHeight) / 4);

	// std::cout << "Constructed WorldDynamic size of: " << sizeof(*this) << '\n';
}

WorldDynamic::~WorldDynamic()
{
	// std::cout << std::format("During the game overall deleted: {} enemies.\n", _overallDeleted);
	// std::cout << std::format("Game left with enemies container of size: {}.\n", _Enemies.size());
}

UniverseDot WorldDynamic::TraverseNewPlatforms(long unsigned int index) { return UniverseDot(); }

void WorldDynamic::OnSpawnNewActor(const int& x, const int& y, const unsigned int& variance) {}

void WorldDynamic::CheckForSpawnNewEnemies(long unsigned int* new_platforms_quantity)
{
	//? Here we clear container with empty places for wormholes
	//? If there would be new places after enemy spawn
	//? container will be populated again
	_emptyPlatformsIndecies.clear();

	if (new_platforms_quantity && (*new_platforms_quantity != 0) && _staticWorld)
	{
		//? Init counter, go thru all available indecies on the back
		int Counter = static_cast<int>(*new_platforms_quantity - 1);	// N-1, ..., 3, 2, 1, 0

		while (Counter >= 0)
		{
			//? Here available places for wormholes could be populated
			DoIndexTraversing(Counter, _enemySpawner.get(), &WorldDynamic::OnSpawnNewEnemy,
							  &WorldDynamic::OnSaveAvailablePlaces);
		}

		//? Clear new_platform_quantity
		*new_platforms_quantity = 0;
	}
}

void WorldDynamic::CheckForSpawnNewWormholes()
{
	for (auto& Index : _emptyPlatformsIndecies)
	{
		auto CopyIndex = Index;	   //? To respect traverser signature
		//? Last argument is nullptr because we don't save empty spaces for anyone else
		DoIndexTraversing(CopyIndex, _wormholeSpawner.get(), &WorldDynamic::OnSpawnNewWormhole, nullptr);
		//* CopyIndex were decremented, but we don't need this functional
		//* and we can't rid of this functional because enemy spawning need this
	}
}

void WorldDynamic::OnSpawnNewEnemy(const int& x, const int& y, const unsigned int& variance)
{
	//? Create sprite with image variance as index
	std::string Path(_enemiesPathCut + std::to_string(variance + 1) + _enemiesPathAdder);
	_enemies.push_back(std::make_unique<Image>(_screen, Path.c_str(),	 //
											   Sizes::Environment::EnemyWidth[variance],
											   Sizes::Environment::EnemyHeight[variance]));
	OnAddNewActorToPlayerCollider(_enemies.back().get());	 //? Don't forget to tell player

	//? Place image in the x,y Bottom-Center coordinate
	_enemyLocators.push_back(std::make_unique<Locator>(_enemies.back().get()));
	_enemyLocators.back()->SetBottomCLocation(x, y);

	OnAddNewActorToScene(_enemyLocators.back().get());	  //? Don't forget to tell scene
}

void WorldDynamic::OnSpawnNewWormhole(const int& x, const int& y, const unsigned int& variance)
{
	// variance //! unused here

	//? Create sprite with image variance as index
	_wormholes.push_back(std::make_unique<Image>(_screen, _wormholePath,			   //
												 Sizes::Environment::WormholeWidth,	   //
												 Sizes::Environment::WormholeHeight));
	OnAddNewWormholeToPlayerCollider(_wormholes.back().get());	  //? Don't forget to tell player

	//? Place image in the x,y Bottom-Center coordinate
	_wormholesLocators.push_back(std::make_unique<Locator>(_wormholes.back().get()));
	_wormholesLocators.back()->SetBottomCLocation(x, y);

	OnAddNewWormholeToScene(_wormholesLocators.back().get());	 //? Don't forget to tell scene

	//* And we use wormhole engine
	_wormholesEngines.push_back(
		std::make_unique<Wormhole>(_screen, _playerEngine, _playerBody, _wormholes.back().get()));
}

void WorldDynamic::OnSpawnNewMoveableActor(const Point& begin, const Point& end)
{
	_ammoTiles.push_back(std::make_unique<Ammo>(_screen, begin, end, _ammoPath));
	//? Ammo is a relocatable actor
	//? And it is implement it's relocate() according to own structure
	OnAddNewAmmoToScene(_ammoTiles.back().get());
	//? Collider also should know that we have added new actor
	OnAddNewAmmoToPlayerCollider(_ammoTiles.back()->GetBoundary(), _ammoTiles.back()->GetEngine());
}

void WorldDynamic::Cleaner()
{
	long unsigned int count{};

	while (!_enemies.empty())
	{
		//! bottom()
		if (_enemies.begin()->get()->Bottom() >= ((int)_screen->_applicationHeight + 4 * _thresholdY))
		{
			_enemyLocators.erase(_enemyLocators.begin());
			OnRemoveFrontActorFromScene();
			_enemies.erase(_enemies.begin());
			OnRemoveFrontActorFromPlayerCollider();

			count++;
		}
		else
		{
			// if (count)
			// {
			// 	_overallDeleted += count;
			// 	LogLine("\t\t\tdeleted {} enemies.\n", count);
			// }
			break;
		}
	}

	while (!_ammoTiles.empty())
	{
		//! bottom()
		if (_ammoTiles.begin()->get()->GetBoundary()->Bottom() >= ((int)_screen->_applicationHeight + 4 * _thresholdY))
		{
			OnRemoveFrontAmmoFromScene();
			OnRemoveFrontAmmoFromPlayerCollider();
			_ammoTiles.erase(_ammoTiles.begin());

			// _overallDeletedAmmos++;
			// LogLine("\t\t\tdeleted {} ammos.\n", _overallDeletedAmmos);
		}
		else
		{
			break;
		}
	}

	while (!_wormholes.empty())
	{
		//! bottom()
		if (_wormholes.begin()->get()->Bottom() >= ((int)_screen->_applicationHeight + 4 * _thresholdY))
		{
			OnRemoveFrontWormholeFromScene();
			OnRemoveFrontWormholeFromPlayerCollider();
			_wormholesEngines.erase(_wormholesEngines.begin());
			_wormholesLocators.erase(_wormholesLocators.begin());
			_wormholes.erase(_wormholes.begin());

			// _overallDeletedWormholes++;
			// LogLine("\t\t\tdeleted {} wormholes.\n", _overallDeletedWormholes);
		}
		else
		{
			break;
		}
	}
}

void WorldDynamic::Tick(float delta_t)
{
	for (auto& Ammo : _ammoTiles)
	{
		Ammo->Tick(delta_t);
	}

	//! TESTS: deletes ammo that gets to target point
	// for (long unsigned int index{}; index < _AmmoTiles.size();)
	// {
	// 	if (_AmmoTiles[index]->IsOnPlace())
	// 	{
	// 		onRemoveAmmoMissed(index);
	// 	}
	// 	else
	// 	{
	// 		index++;
	// 	}
	// }

	for (auto& WormholeEngine : _wormholesEngines)
	{
		WormholeEngine->Tick(delta_t);
		if ((WormholeEngine->IsStucked()) && _level)
		{
			_level->OnLevelStop();
		}
	}
}

void WorldDynamic::SetLevelConnection(IStoppable* new_level) { _level = new_level; }

void WorldDynamic::SetStaticWorldConnection(IUniverse* new_world) { _staticWorld = new_world; }

void WorldDynamic::SetSceneConnection(ILocatableScene* new_scene) { _scene = new_scene; }

void WorldDynamic::SetPlayerColliderConnection(ICollidable* new_collider) { _playerCollider = new_collider; }

void WorldDynamic::SetPlayerBodyConnection(RectangleShape* character_body) { _playerBody = character_body; }

void WorldDynamic::SetPlayerEngineConnection(IMechanics* character_engine) { _playerEngine = character_engine; }

void WorldDynamic::Initialize()
{
	// std::cout << "Initializing WorldDynamic...\n";

	//* How we spawn new enemies
	//? Later we will use spawner to create new enemy in the world
	_enemySpawner = std::make_unique<RandomSpawn>(this);
	//? Offset by Y will be used to roll dice on spawn: right>left == spawn
	_enemySpawner->SetOffsetsY(10, 23);	   // TODO: get rid of magic number
	//? Offset by X will be used to roll dice on what type of enemy to spawn
	//? there would be enemies with indecies: [0, 4] - 5 overall
	_enemySpawner->SetOffsetsX(0, 4);	 // TODO: get rid of magic number

	//* How we spawn new wormholes
	//? Later we will use spawner to create new wormhole in the world
	_wormholeSpawner = std::make_unique<RandomSpawn>(this);
	//? Offset by Y will be used to roll dice on spawn: right>left == spawn
	_wormholeSpawner->SetOffsetsY(2, 67);	 // TODO: get rid of magic number
	//? Offset by X will be used to roll dice on what type of wormhole to spawn
	//? there would be only one wormhole with index '0'
	_wormholeSpawner->SetOffsetsX(0, 0);	// TODO: get rid of magic number
}

void WorldDynamic::OnEnemyKilled(long unsigned int index)
{
	//? Tell scene index of killed enemy
	if (_scene)
	{
		_scene->OnEnemyKilled(index);
	}
	//? Delete here
	if (!_enemies.empty())
	{
		_enemies.erase(_enemies.begin() + index);
		// _overallDeleted++;
	}
}

void WorldDynamic::OnAmmoDestroyed(long unsigned int index)
{
	//? Tell scene index of destroyed ammo
	if (_scene)
	{
		_scene->OnAmmoDestroyed(index);
	}
	//? Delete here
	if (!_ammoTiles.empty())
	{
		_ammoTiles.erase(_ammoTiles.begin() + index);
		// _overallDeletedAmmos++;
	}
}

void WorldDynamic::Render()
{
	for (auto& Enemy : _enemies)
	{
		Enemy.get()->Render();
	}
	for (auto& AmmoTile : _ammoTiles)
	{
		AmmoTile.get()->Render();
	}
	for (auto& Wormhole : _wormholes)
	{
		Wormhole.get()->Render();
	}
}

void WorldDynamic::OnAddNewActorToScene(IRelocatableActor* new_actor)
{
	if (_scene)
	{
		_scene->AddRelocatableEnemy(new_actor);
	}
}

void WorldDynamic::OnAddNewActorToPlayerCollider(RectangleCore* new_actor)
{
	if (_playerCollider)
	{
		_playerCollider->AddBlockedEnemy(new_actor);
	}
}

void WorldDynamic::OnAddNewWormholeToScene(IRelocatableActor* new_actor)
{
	if (_scene)
	{
		_scene->AddRelocatableWormhole(new_actor);
	}
}

void WorldDynamic::OnAddNewWormholeToPlayerCollider(RectangleCore* new_actor)
{
	if (_playerCollider)
	{
		_playerCollider->AddBlockedWormhole(new_actor);
	}
}

void WorldDynamic::OnAddNewAmmoToScene(IRelocatableActor* new_actor)
{
	if (_scene)
	{
		_scene->AddRelocatableAmmo(new_actor);
	}
}

void WorldDynamic::OnAddNewAmmoToPlayerCollider(RectangleCore* new_actor, IMechanics* engine)
{
	if (_playerCollider)
	{
		_playerCollider->AddShooterAmmo(new_actor, engine);
	}
}

void WorldDynamic::OnRemoveFrontActorFromScene()
{
	if (_scene)
	{
		_scene->RemoveRelocatableEnemyAtFront();
	}
}

void WorldDynamic::OnRemoveFrontActorFromPlayerCollider()
{
	if (_playerCollider)
	{
		_playerCollider->RemoveEnemyAtFront();
	}
}

void WorldDynamic::OnRemoveFrontWormholeFromScene()
{
	if (_scene)
	{
		_scene->RemoveRelocatableWormholeAtFront();
	}
}

void WorldDynamic::OnRemoveFrontWormholeFromPlayerCollider()
{
	if (_playerCollider)
	{
		_playerCollider->RemoveWormholeAtFront();
	}
}

void WorldDynamic::OnRemoveAmmoMissed(long unsigned int index)
{
	//? First delete from scene
	if (_scene)
	{
		_scene->OnAmmoMissed(index);
	}
	//? Delete here
	if (!_ammoTiles.empty())
	{
		_ammoTiles.erase(_ammoTiles.begin() + index);
		// _overallDeleted++;//TODO: maybe counter here
	}
}

void WorldDynamic::OnRemoveFrontAmmoFromScene()
{
	if (_scene)
	{
		_scene->RemoveRelocatableAmmoAtFront();
	}
}

void WorldDynamic::OnRemoveFrontAmmoFromPlayerCollider()
{
	if (_playerCollider)
	{
		_playerCollider->RemoveAmmoAtFront();
	}
}

void WorldDynamic::DoIndexTraversing(int& counter, RandomSpawn* spawner, SpawnCallBackType onSpawn,
									 ContainerPopulationCallBackType onSave)
{
	//? Do we skip this index?
	if (!spawner->DoWeSpawnByHeight())
	{
		if (onSave)
		{
			//? Another actor could be placed here
			(this->*onSave)(counter);
		}
		counter--;
		return;
	}

	//? What image we will be using
	int ImageVariant = spawner->OrderVariantByWidth();
	//? Traverse new platforms: post-condition coordinates to place Enemy will be given
	auto NewPlacement = _staticWorld->TraverseNewPlatforms(counter);

	(this->*onSpawn)(NewPlacement.x, NewPlacement.y, ImageVariant);
	counter--;
}

void WorldDynamic::OnSaveAvailablePlaces(const int available_place_to_spawn_wormhole)
{
	_emptyPlatformsIndecies.push_back(available_place_to_spawn_wormhole);
}
