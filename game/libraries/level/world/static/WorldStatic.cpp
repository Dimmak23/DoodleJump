//* Source header
#include "WorldStatic.hpp"

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
//? Modules
#include "Locator.hpp"
#include "RandomSpawn.hpp"
//? Wrappers
#include "SpriteWrapper.hpp"
//? Abilities
#include "Stealth.hpp"
//? Utilities
#include "Environment.hpp"
#include "Sizes.hpp"

WorldStatic::WorldStatic(const ScreenItem* parent_screen,	 //
						 const char* path_cut,				 //
						 const char* path_adder,			 //
						 const unsigned int quantity)
	: _Screen(parent_screen)
	, _PlatformWidth(parent_screen->ScaleWidth * Sizes::Environment::PlatformWidth)
	, _PlatformHeight(parent_screen->ScaleHeight * Sizes::Environment::PlatformHeight)
	, _PlatformsPathCut(path_cut)
	, _PlatformsPathAdder(path_adder)
	, _PlatformsQuantity(quantity)
{
	_ThresholdY = float(_Screen->ApplicationHeight) / 4;

	// std::cout << std::format("Constructed WorldStatic size of: {}\n", sizeof(*this));
}

WorldStatic::~WorldStatic()
{
	// std::cout << std::format("During the game overall deleted: {} platforms.\n", _OverallDeleted);
	// std::cout << std::format("Game left with platforms container of size: {}.\n", _Platforms.size());

	// std::cout << "deleted _PlatformSpawner...\n";
}

size_t WorldStatic::getDeletedPlatformsCount() { return _OverallDeleted; }

size_t WorldStatic::getAllBellowPlatformsCount(const int& actor_bottom)
{
	size_t result{};
	for (auto& Platform : _Platforms)
	{
		if (Platform->top() >= actor_bottom)
		{
			result++;
		}
	}

	return result;
}

UniverseDot WorldStatic::traverseNewPlatforms(size_t index)
{
	if (!_DynamicWorld) return UniverseDot();

	//? Need coordinates to place Enemy
	//? index will be changing from N-1 to 0
	//? going from bottom to top (.begin() at bottom, .end() at top)
	auto LocatorIterator = _PlatformsLocators.end() - 1 - index;
	// TODO: a lot of types such Deltas, Point, Locator::Coordinate, UniverseDot
	// TODO: which is the same, and only one should be used everywhere
	Locator::Coordinate NewPlacement = LocatorIterator->get()->getTopCLocation();

	return UniverseDot{ NewPlacement.x, NewPlacement.y };
}

void WorldStatic::setDynamicWorldConnection(IUniverse* new_world) { _DynamicWorld = new_world; }

void WorldStatic::setSceneConnection(ILocatableScene* new_scene) { _Scene = new_scene; }

void WorldStatic::setPlayerColliderConnection(ICollidable* new_collider) { _PlayerCollider = new_collider; }

int WorldStatic::getPlatformsBottom() const { return _Platforms.begin()->get()->bottom(); }

Point WorldStatic::getFirstPlatformTopCLocation() const
{
	return Point(_Platforms.begin()->get()->center().x, _Platforms.begin()->get()->top());
}

size_t* WorldStatic::getNewPlatformsQuantity() { return &_NewPlatformsQuantity; }

void WorldStatic::initialize()
{
	//? Later we will use spawner to create new platform in the world
	// TODO: terrible initializing
	// TODO: maybe set platforms width and height variables on the construction
	_PlatformSpawner = std::make_unique<RandomSpawn>(this, _PlatformWidth, _PlatformHeight);

	//? How far new plaforms would be offseted
	_PlatformSpawner->setOffsetsX(_PlatformWidth + 20, 20);	   // TODO: get rid of magic number
	_PlatformSpawner->setOffsetsY(6 * _PlatformHeight, 40);	   // TODO: get rid of magic number

	//? Initially we create only one platform for player stand on it
	_PlatformSpawner->orderSingleActor(_Screen->ApplicationHeight, _Screen->ApplicationWidth, _PlatformsQuantity,
									   &IUniverse::onSpawnNewActor);

	//? And spawning another set of platforms so player not be boring :)
	checkForSpawnNewPlatforms(0);

	_Stealth = std::make_unique<Stealth>();
	_Stealth->resetSpriteConnection(_Platforms[0].get());
}

// TODO: call this method in the initializer to spawn first platform for player
void WorldStatic::onSpawnNewActor(const int& x, const int& y, const unsigned int& variance)
{
	std::string Path(_PlatformsPathCut + std::to_string(variance + 1) + _PlatformsPathAdder);
	_Platforms.push_back(std::make_unique<SpriteWrapper>(_Screen, Path.c_str(),	   //
														 Sizes::Environment::PlatformWidth,
														 Sizes::Environment::PlatformHeight));
	onAddNewActorToPlayerCollider(_Platforms.back().get());	   //? Don't forget to tell player

	_PlatformsLocators.push_back(std::make_unique<Locator>(_Platforms.back().get()));
	_PlatformsLocators.back()->setLBCornerLocation(x, y);
	onAddNewActorToScene(_PlatformsLocators.back().get());	  //? Don't forget to tell scene
}

void WorldStatic::checkForSpawnNewPlatforms(const int& player_y)
{
	if (_Platforms.empty()) return;

	if (player_y < (_Platforms.back()->top() + _ThresholdY))
	{
		//? Ordering new actors
		// TODO: get rid of magic number
		Rectangle NewActorsArea{
			0,																									  // x
			_Platforms.back()->top() - (int)_Screen->ApplicationHeight - 7 * (int)_Platforms.back()->height(),	  // y
			int(_Screen->ApplicationWidth), int(_Screen->ApplicationHeight)	   // size
		};
		_NewPlatformsQuantity =
			_PlatformSpawner->orderNewActors(NewActorsArea, _PlatformsQuantity, &IUniverse::onSpawnNewActor);
	}
}

void WorldStatic::cleaner()
{
	if (_Platforms.empty()) return;

	size_t count{};

	while (true)
	{
		if (_Platforms.begin()->get()->top() >= ((int)_Screen->ApplicationHeight + 4 * _ThresholdY))
		{
			// std::cout << std::format("{} >= {}. erasing...\n", _Platforms.begin()->get()->top(),
			// 						 (int)_appHeight + 4*_ThresholdY);
			_PlatformsLocators.erase(_PlatformsLocators.begin());
			onRemoveFrontActorFromScene();
			_Platforms.erase(_Platforms.begin());
			onRemoveFrontActorFromPlayerCollider();

			count++;
		}
		else
		{
			if (count)
			{
				_OverallDeleted += count;
				// std::cout << std::format("\t\tdeleted {} actors.\n", count);
			}
			break;
		}
		// PlatformIterator++;
	}
}

void WorldStatic::tick(float delta_t) { _Stealth->tick(delta_t); }

void WorldStatic::render()
{
	for (auto& Platform : _Platforms)
	{
		Platform->render();
	}
}

void WorldStatic::onTopOfPlatformMessaging(size_t index)
{
	//? Checking if module not taking by platform already
	if (_Stealth->getPlatformImage() != dynamic_cast<IGraphicle*>(_Platforms[index].get()))
	{
		_Stealth->resetSpriteConnection(_Platforms[index].get());
	}
}

void WorldStatic::onFlyingMessaging() { _Stealth->emptySpriteConnection(); }

void WorldStatic::onAddNewActorToScene(IRelocatableActor* new_actor)
{
	if (_Scene)
	{
		_Scene->addRelocatableActor(new_actor);
	}
}

void WorldStatic::onAddNewActorToPlayerCollider(RectangleCore* new_actor)
{
	if (_PlayerCollider)
	{
		_PlayerCollider->addBlockedActor(new_actor);
	}
}

void WorldStatic::onRemoveFrontActorFromScene()
{
	if (_Scene)
	{
		_Scene->removeRelocatableActorAtFront();
	}
}

void WorldStatic::onRemoveFrontActorFromPlayerCollider()
{
	if (_PlayerCollider)
	{
		_PlayerCollider->removeBlockedActorAtFront();
	}
}
