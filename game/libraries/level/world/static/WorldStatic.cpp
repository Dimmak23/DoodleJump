//* Source header
#include "level/world/static/WorldStatic.hpp"

//* Game
#include "abilities/stealth/Stealth.hpp"
#include "graphics/image/Image.hpp"
#include "modules/collider/I_Collidable.hpp"
#include "modules/locator/Locator.hpp"
#include "modules/scene/I_LocatableScene.hpp"
#include "modules/spawn/RandomSpawn.hpp"
#include "screen/ScreenItem.hpp"
#include "utilities/Environment.hpp"
#include "utilities/Sizes.hpp"

//* C++ std
#include <format>
#include <iostream>
#include <string>

WorldStatic::WorldStatic(const ScreenItem* parent_screen,	 //
						 const char* path_cut,				 //
						 const char* path_adder,			 //
						 const unsigned int quantity)
	: _screen(parent_screen)
	, _platformWidth((const unsigned int)(parent_screen->_scaleWidth * Sizes::Environment::PlatformWidth))
	, _platformHeight((const unsigned int)(parent_screen->_scaleHeight * Sizes::Environment::PlatformHeight))
	, _platformsPathCut(path_cut)
	, _platformsPathAdder(path_adder)
	, _platformsQuantity(quantity)
{
	_thresholdY = int(float(_screen->_applicationHeight) / 4);

	// std::cout << std::format("Constructed WorldStatic size of: {}\n", sizeof(*this));
}

WorldStatic::~WorldStatic()
{
	// std::cout << std::format("During the game overall deleted: {} platforms.\n", _OverallDeleted);
	// std::cout << std::format("Game left with platforms container of size: {}.\n", _Platforms.size());

	// std::cout << "deleted _PlatformSpawner...\n";
}

long unsigned int WorldStatic::GetDeletedPlatformsCount() { return _overallDeleted; }

long unsigned int WorldStatic::GetAllBellowPlatformsCount(const int& actor_bottom)
{
	size_t result{};
	for (auto& Platform : _platforms)
	{
		if (Platform->Top() >= actor_bottom)
		{
			result++;
		}
	}

	return result;
}

UniverseDot WorldStatic::TraverseNewPlatforms(long unsigned int index)
{
	if (!_dynamicWorld) return UniverseDot();

	//? Need coordinates to place Enemy
	//? index will be changing from N-1 to 0
	//? going from bottom to top (.begin() at bottom, .end() at top)
	auto LocatorIterator = _platformsLocators.end() - 1 - index;
	// TODO: a lot of types such Deltas, Point, Locator::Coordinate, UniverseDot
	// TODO: which is the same, and only one should be used everywhere
	Locator::Coordinate NewPlacement = LocatorIterator->get()->GetTopCLocation();

	return UniverseDot{ NewPlacement.x, NewPlacement.y };
}

void WorldStatic::SetDynamicWorldConnection(IUniverse* new_world) { _dynamicWorld = new_world; }

void WorldStatic::SetSceneConnection(ILocatableScene* new_scene) { _scene = new_scene; }

void WorldStatic::SetPlayerColliderConnection(ICollidable* new_collider) { _playerCollider = new_collider; }

int WorldStatic::GetPlatformsBottom() const { return _platforms.begin()->get()->Bottom(); }

Point WorldStatic::GetFirstPlatformTopCLocation() const
{
	return Point(_platforms.begin()->get()->Center().x, _platforms.begin()->get()->Top());
}

long unsigned int* WorldStatic::GetNewPlatformsQuantity() { return &_newPlatformsQuantity; }

void WorldStatic::Initialize()
{
	//? Later we will use spawner to create new platform in the world
	// TODO: terrible initializing
	// TODO: maybe set platforms width and height variables on the construction
	_platformSpawner = std::make_unique<RandomSpawn>(this, _platformWidth, _platformHeight);

	//? How far new plaforms would be offseted
	_platformSpawner->SetOffsetsX(_platformWidth + 20, 20);	   // TODO: get rid of magic number
	_platformSpawner->SetOffsetsY(6 * _platformHeight, 40);	   // TODO: get rid of magic number

	//? Initially we create only one platform for player stand on it
	_platformSpawner->OrderSingleActor(_screen->_applicationHeight, _screen->_applicationWidth, _platformsQuantity,
									   &IUniverse::OnSpawnNewActor);

	//? And spawning another set of platforms so player not be boring :)
	CheckForSpawnNewPlatforms(0);

	_stealth = std::make_unique<Stealth>();
	_stealth->ResetSpriteConnection(_platforms[0].get());
}

// TODO: call this method in the initializer to spawn first platform for player
void WorldStatic::OnSpawnNewActor(const int& x, const int& y, const unsigned int& variance)
{
	std::string Path(_platformsPathCut + std::to_string(variance + 1) + _platformsPathAdder);
	_platforms.push_back(std::make_unique<Image>(_screen, Path.c_str(),	   //
												 Sizes::Environment::PlatformWidth,
												 Sizes::Environment::PlatformHeight));
	OnAddNewActorToPlayerCollider(_platforms.back().get());	   //? Don't forget to tell player

	_platformsLocators.push_back(std::make_unique<Locator>(_platforms.back().get()));
	_platformsLocators.back()->SetLBCornerLocation(x, y);
	OnAddNewActorToScene(_platformsLocators.back().get());	  //? Don't forget to tell scene
}

void WorldStatic::CheckForSpawnNewPlatforms(const int& player_y)
{
	if (_platforms.empty()) return;

	if (player_y < (_platforms.back()->Top() + _thresholdY))
	{
		//? Ordering new actors
		// TODO: get rid of magic number
		Rectangle NewActorsArea{
			0,																									   // x
			_platforms.back()->Top() - (int)_screen->_applicationHeight - 7 * (int)_platforms.back()->Height(),	   // y
			int(_screen->_applicationWidth), int(_screen->_applicationHeight)	 // size
		};
		_newPlatformsQuantity =
			_platformSpawner->OrderNewActors(NewActorsArea, _platformsQuantity, &IUniverse::OnSpawnNewActor);
	}
}

void WorldStatic::Cleaner()
{
	if (_platforms.empty()) return;

	size_t count{};

	while (true)
	{
		if (_platforms.begin()->get()->Top() >= ((int)_screen->_applicationHeight + 4 * _thresholdY))
		{
			// std::cout << std::format("{} >= {}. erasing...\n", _Platforms.begin()->get()->top(),
			// 						 (int)_appHeight + 4*_ThresholdY);
			_platformsLocators.erase(_platformsLocators.begin());
			OnRemoveFrontActorFromScene();
			_platforms.erase(_platforms.begin());
			OnRemoveFrontActorFromPlayerCollider();

			count++;
		}
		else
		{
			if (count)
			{
				_overallDeleted += count;
				// std::cout << std::format("\t\tdeleted {} actors.\n", count);
			}
			break;
		}
		// PlatformIterator++;
	}
}

void WorldStatic::Tick(float delta_t) { _stealth->Tick(delta_t); }

void WorldStatic::Render()
{
	for (auto& Platform : _platforms)
	{
		Platform->Render();
	}
}

void WorldStatic::OnTopOfPlatformMessaging(long unsigned int index)
{
	//? Checking if module not taking by platform already
	if (_stealth->GetPlatformImage() != dynamic_cast<IGraphicle*>(_platforms[index].get()))
	{
		_stealth->ResetSpriteConnection(_platforms[index].get());
	}
}

void WorldStatic::OnFlyingMessaging() { _stealth->EmptySpriteConnection(); }

void WorldStatic::OnAddNewActorToScene(IRelocatableActor* new_actor)
{
	if (_scene)
	{
		_scene->AddRelocatableActor(new_actor);
	}
}

void WorldStatic::OnAddNewActorToPlayerCollider(RectangleCore* new_actor)
{
	if (_playerCollider)
	{
		_playerCollider->AddBlockedActor(new_actor);
	}
}

void WorldStatic::OnRemoveFrontActorFromScene()
{
	if (_scene)
	{
		_scene->RemoveRelocatableActorAtFront();
	}
}

void WorldStatic::OnRemoveFrontActorFromPlayerCollider()
{
	if (_playerCollider)
	{
		_playerCollider->RemoveBlockedActorAtFront();
	}
}
