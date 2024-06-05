//* Source header
#include "WorldDynamic.hpp"

//* C++ std
#include <format>
#include <iostream>
#include <string>

//* Custom
//? Inheritance
#include "ScreenItem.hpp"
//? Interfaces
#include "I_Collidable.hpp"
#include "I_LocatableScene.hpp"
#include "I_Mechanics.hpp"
#include "I_Stoppable.hpp"
//? Modules
#include "Locator.hpp"
#include "PhysicsEngine.hpp"
#include "RandomSpawn.hpp"
//? Wrappers
#include "SpriteWrapper.hpp"
//? Projectiles
#include "Ammo.hpp"
//? Abilities
#include "Wormhole.hpp"
//? Utilities
#include "Sizes.hpp"

WorldDynamic::WorldDynamic(const ScreenItem* parent_screen,		   //
						   const char* enemies_path_cut,		   //
						   const char* enemies_path_adder,		   //
						   const unsigned int enemies_quantity,	   //
						   const char* wormhole_path,			   //
						   const char* ammo_path)
	: _Screen(parent_screen)
	, _EnemiesPathCut(enemies_path_cut)
	, _EnemiesPathAdder(enemies_path_adder)
	, _EnemiesQuantity(enemies_quantity)
	, _WormholePath(wormhole_path)
	, _AmmoPath(ammo_path)
{
	_ThresholdY = int(float(_Screen->ApplicationHeight) / 4);

	// std::cout << "Constructed WorldDynamic size of: " << sizeof(*this) << '\n';
}

WorldDynamic::~WorldDynamic()
{
	// std::cout << std::format("During the game overall deleted: {} enemies.\n", _OverallDeleted);
	// std::cout << std::format("Game left with enemies container of size: {}.\n", _Enemies.size());
}

UniverseDot WorldDynamic::traverseNewPlatforms(size_t index) { return UniverseDot(); }

void WorldDynamic::onSpawnNewActor(const int& x, const int& y, const unsigned int& variance) {}

void WorldDynamic::checkForSpawnNewEnemies(size_t* new_platforms_quantity)
{
	//? Here we clear container with empty places for wormholes
	//? If there would be new places after enemy spawn
	//? container will be populated again
	_EmptyPlatformsIndecies.clear();

	if (new_platforms_quantity && (*new_platforms_quantity != 0) && _StaticWorld)
	{
		//? Init counter, go thru all available indecies on the back
		int Counter = static_cast<int>(*new_platforms_quantity - 1);	// N-1, ..., 3, 2, 1, 0

		while (Counter >= 0)
		{
			//? Here available places for wormholes could be populated
			doIndexTraversing(Counter, _EnemySpawner.get(), &WorldDynamic::onSpawnNewEnemy,
							  &WorldDynamic::onSaveAvailablePlaces);
		}

		//? Clear new_platform_quantity
		*new_platforms_quantity = 0;
	}
}

void WorldDynamic::checkForSpawnNewWormholes()
{
	for (auto& Index : _EmptyPlatformsIndecies)
	{
		auto CopyIndex = Index;	   //? To respect traverser signature
		//? Last argument is nullptr because we don't save empty spaces for anyone else
		doIndexTraversing(CopyIndex, _WormholeSpawner.get(), &WorldDynamic::onSpawnNewWormhole, nullptr);
		//* CopyIndex were decremented, but we don't need this functional
		//* and we can't rid of this functional because enemy spawning need this
	}
}

void WorldDynamic::onSpawnNewEnemy(const int& x, const int& y, const unsigned int& variance)
{
	//? Create sprite with image variance as index
	std::string Path(_EnemiesPathCut + std::to_string(variance + 1) + _EnemiesPathAdder);
	_Enemies.push_back(std::make_unique<SpriteWrapper>(_Screen, Path.c_str(),	 //
													   Sizes::Environment::EnemyWidth[variance],
													   Sizes::Environment::EnemyHeight[variance]));
	onAddNewActorToPlayerCollider(_Enemies.back().get());	 //? Don't forget to tell player

	//? Place image in the x,y Bottom-Center coordinate
	_EnemyLocators.push_back(std::make_unique<Locator>(_Enemies.back().get()));
	_EnemyLocators.back()->setBottomCLocation(x, y);

	onAddNewActorToScene(_EnemyLocators.back().get());	  //? Don't forget to tell scene
}

void WorldDynamic::onSpawnNewWormhole(const int& x, const int& y, const unsigned int& variance)
{
	// variance //! unused here

	//? Create sprite with image variance as index
	_Wormholes.push_back(std::make_unique<SpriteWrapper>(_Screen, _WormholePath,			   //
														 Sizes::Environment::WormholeWidth,	   //
														 Sizes::Environment::WormholeHeight));
	onAddNewWormholeToPlayerCollider(_Wormholes.back().get());	  //? Don't forget to tell player

	//? Place image in the x,y Bottom-Center coordinate
	_WormholesLocators.push_back(std::make_unique<Locator>(_Wormholes.back().get()));
	_WormholesLocators.back()->setBottomCLocation(x, y);

	onAddNewWormholeToScene(_WormholesLocators.back().get());	 //? Don't forget to tell scene

	//* And we use wormhole engine
	_WormholesEngines.push_back(
		std::make_unique<Wormhole>(_Screen, _PlayerEngine, _PlayerBody, _Wormholes.back().get()));
}

void WorldDynamic::onSpawnNewMoveableActor(const Point& begin, const Point& end)
{
	_AmmoTiles.push_back(std::make_unique<Ammo>(_Screen, begin, end, _AmmoPath));
	//? Ammo is a relocatable actor
	//? And it is implement it's relocate() according to own structure
	onAddNewAmmoToScene(_AmmoTiles.back().get());
	//? Collider also should know that we have added new actor
	onAddNewAmmoToPlayerCollider(_AmmoTiles.back()->getBoundary(), _AmmoTiles.back()->getEngine());
}

void WorldDynamic::cleaner()
{
	size_t count{};

	while (!_Enemies.empty())
	{
		//! bottom()
		if (_Enemies.begin()->get()->bottom() >= ((int)_Screen->ApplicationHeight + 4 * _ThresholdY))
		{
			_EnemyLocators.erase(_EnemyLocators.begin());
			onRemoveFrontActorFromScene();
			_Enemies.erase(_Enemies.begin());
			onRemoveFrontActorFromPlayerCollider();

			count++;
		}
		else
		{
			if (count)
			{
				_OverallDeleted += count;
				// std::cout << std::format("\t\t\tdeleted {} enemies.\n", count);
			}
			break;
		}
	}

	while (!_AmmoTiles.empty())
	{
		//! bottom()
		if (_AmmoTiles.begin()->get()->getBoundary()->bottom() >= ((int)_Screen->ApplicationHeight + 4 * _ThresholdY))
		{
			onRemoveFrontAmmoFromScene();
			onRemoveFrontAmmoFromPlayerCollider();
			_AmmoTiles.erase(_AmmoTiles.begin());

			_OverallDeletedAmmos++;
			// std::cout << std::format("\t\t\tdeleted {} ammos.\n", _OverallDeletedAmmos);
		}
		else
		{
			break;
		}
	}

	while (!_Wormholes.empty())
	{
		//! bottom()
		if (_Wormholes.begin()->get()->bottom() >= ((int)_Screen->ApplicationHeight + 4 * _ThresholdY))
		{
			onRemoveFrontWormholeFromScene();
			onRemoveFrontWormholeFromPlayerCollider();
			_WormholesEngines.erase(_WormholesEngines.begin());
			_WormholesLocators.erase(_WormholesLocators.begin());
			_Wormholes.erase(_Wormholes.begin());

			_OverallDeletedWormholes++;
			// std::cout << std::format("\t\t\tdeleted {} wormholes.\n", _OverallDeletedWormholes);
		}
		else
		{
			break;
		}
	}
}

void WorldDynamic::tick(float delta_t)
{
	for (auto& Ammo : _AmmoTiles)
	{
		Ammo->tick(delta_t);
	}

	//! TESTS: deletes ammo that gets to target point
	// for (size_t index{}; index < _AmmoTiles.size();)
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

	for (auto& WormholeEngine : _WormholesEngines)
	{
		WormholeEngine->tick(delta_t);
		if ((WormholeEngine->IsStucked()) && _Level)
		{
			_Level->onLevelStop();
		}
	}
}

void WorldDynamic::setLevelConnection(IStoppable* new_level) { _Level = new_level; }

void WorldDynamic::setStaticWorldConnection(IUniverse* new_world) { _StaticWorld = new_world; }

void WorldDynamic::setSceneConnection(ILocatableScene* new_scene) { _Scene = new_scene; }

void WorldDynamic::setPlayerColliderConnection(ICollidable* new_collider) { _PlayerCollider = new_collider; }

void WorldDynamic::setPlayerBodyConnection(RectangleShape* character_body) { _PlayerBody = character_body; }

void WorldDynamic::setPlayerEngineConnection(IMechanics* character_engine) { _PlayerEngine = character_engine; }

void WorldDynamic::initialize()
{
	// std::cout << "Initializing WorldDynamic...\n";

	//* How we spawn new enemies
	//? Later we will use spawner to create new enemy in the world
	_EnemySpawner = std::make_unique<RandomSpawn>(this);
	//? Offset by Y will be used to roll dice on spawn: right>left == spawn
	_EnemySpawner->setOffsetsY(10, 23);	   // TODO: get rid of magic number
	//? Offset by X will be used to roll dice on what type of enemy to spawn
	//? there would be enemies with indecies: [0, 4] - 5 overall
	_EnemySpawner->setOffsetsX(0, 4);	 // TODO: get rid of magic number

	//* How we spawn new wormholes
	//? Later we will use spawner to create new wormhole in the world
	_WormholeSpawner = std::make_unique<RandomSpawn>(this);
	//? Offset by Y will be used to roll dice on spawn: right>left == spawn
	_WormholeSpawner->setOffsetsY(2, 67);	 // TODO: get rid of magic number
	//? Offset by X will be used to roll dice on what type of wormhole to spawn
	//? there would be only one wormhole with index '0'
	_WormholeSpawner->setOffsetsX(0, 0);	// TODO: get rid of magic number
}

void WorldDynamic::onEnemyKilled(size_t index)
{
	//? Tell scene index of killed enemy
	if (_Scene)
	{
		_Scene->onEnemyKilled(index);
	}
	//? Delete here
	if (!_Enemies.empty())
	{
		_Enemies.erase(_Enemies.begin() + index);
		_OverallDeleted++;
	}
}

void WorldDynamic::onAmmoDestroyed(size_t index)
{
	//? Tell scene index of destroyed ammo
	if (_Scene)
	{
		_Scene->onAmmoDestroyed(index);
	}
	//? Delete here
	if (!_AmmoTiles.empty())
	{
		_AmmoTiles.erase(_AmmoTiles.begin() + index);
		_OverallDeletedAmmos++;
	}
}

void WorldDynamic::render()
{
	for (auto& Enemy : _Enemies)
	{
		Enemy.get()->render();
	}
	for (auto& AmmoTile : _AmmoTiles)
	{
		AmmoTile.get()->render();
	}
	for (auto& Wormhole : _Wormholes)
	{
		Wormhole.get()->render();
	}
}

void WorldDynamic::onAddNewActorToScene(IRelocatableActor* new_actor)
{
	if (_Scene)
	{
		_Scene->addRelocatableEnemy(new_actor);
	}
}

void WorldDynamic::onAddNewActorToPlayerCollider(RectangleCore* new_actor)
{
	if (_PlayerCollider)
	{
		_PlayerCollider->addBlockedEnemy(new_actor);
	}
}

void WorldDynamic::onAddNewWormholeToScene(IRelocatableActor* new_actor)
{
	if (_Scene)
	{
		_Scene->addRelocatableWormhole(new_actor);
	}
}

void WorldDynamic::onAddNewWormholeToPlayerCollider(RectangleCore* new_actor)
{
	if (_PlayerCollider)
	{
		_PlayerCollider->addBlockedWormhole(new_actor);
	}
}

void WorldDynamic::onAddNewAmmoToScene(IRelocatableActor* new_actor)
{
	if (_Scene)
	{
		_Scene->addRelocatableAmmo(new_actor);
	}
}

void WorldDynamic::onAddNewAmmoToPlayerCollider(RectangleCore* new_actor, IMechanics* engine)
{
	if (_PlayerCollider)
	{
		_PlayerCollider->addShooterAmmo(new_actor, engine);
	}
}

void WorldDynamic::onRemoveFrontActorFromScene()
{
	if (_Scene)
	{
		_Scene->removeRelocatableEnemyAtFront();
	}
}

void WorldDynamic::onRemoveFrontActorFromPlayerCollider()
{
	if (_PlayerCollider)
	{
		_PlayerCollider->removeEnemyAtFront();
	}
}

void WorldDynamic::onRemoveFrontWormholeFromScene()
{
	if (_Scene)
	{
		_Scene->removeRelocatableWormholeAtFront();
	}
}

void WorldDynamic::onRemoveFrontWormholeFromPlayerCollider()
{
	if (_PlayerCollider)
	{
		_PlayerCollider->removeWormholeAtFront();
	}
}

void WorldDynamic::onRemoveAmmoMissed(size_t index)
{
	//? First delete from scene
	if (_Scene)
	{
		_Scene->onAmmoMissed(index);
	}
	//? Delete here
	if (!_AmmoTiles.empty())
	{
		_AmmoTiles.erase(_AmmoTiles.begin() + index);
		// _OverallDeleted++;//TODO: maybe counter here
	}
}

void WorldDynamic::onRemoveFrontAmmoFromScene()
{
	if (_Scene)
	{
		_Scene->removeRelocatableAmmoAtFront();
	}
}

void WorldDynamic::onRemoveFrontAmmoFromPlayerCollider()
{
	if (_PlayerCollider)
	{
		_PlayerCollider->removeAmmoAtFront();
	}
}

void WorldDynamic::doIndexTraversing(int& counter, RandomSpawn* spawner, SpawnCallBackType onSpawn,
									 ContainerPopulationCallBackType onSave)
{
	//? Do we skip this index?
	if (!spawner->doWeSpawnByHeight())
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
	int ImageVariant = spawner->orderVariantByWidth();
	//? Traverse new platforms: post-condition coordinates to place Enemy will be given
	auto NewPlacement = _StaticWorld->traverseNewPlatforms(counter);

	(this->*onSpawn)(NewPlacement.x, NewPlacement.y, ImageVariant);
	counter--;
}

void WorldDynamic::onSaveAvailablePlaces(const int available_place_to_spawn_wormhole)
{
	_EmptyPlatformsIndecies.push_back(available_place_to_spawn_wormhole);
}
