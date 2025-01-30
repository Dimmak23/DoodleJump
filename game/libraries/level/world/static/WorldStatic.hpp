#pragma once

//* Game
#include "level/world/interface/I_Accounting.hpp"
#include "level/world/interface/I_Universe.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class Image;				//? wrappers
class Locator;				//? locator
class IRelocatableActor;	//? locator/interface
class ILocatableScene;		//? scene/interface
class ICollidable;			//? collider/interface
class RectangleCore;		//? inheritance
class RandomSpawn;			//? spawn
struct ScreenItem;
struct Point;
class Stealth;

/*
@ How to use?
@ Construct WorldStatic pointer for application with specific width and height.
@ Set connection to a scene and player collider:
@ 		SetSceneConnection(new_scene), setPlayerColliderConnection(new_collider).
@ Call initialize() create initial world state.
@ Call checkForSpawnNewPlatforms(player_y) every frame to order new platforms if player close to the end of the world.
@ Call cleaner(): maybe we can erase some platforms from memory.
@ Call render() to show in the world spawned actors.
*/
class WorldStatic : public IUniverse, public IAccounting
{
public:
	WorldStatic(const ScreenItem* parent_screen,	//
				const char* path_cut,				//
				const char* path_adder,				//
				const unsigned int quantity);
	virtual ~WorldStatic();

	//@ API for other worlds

	//* Getters

	//? get API for deleted Platforms
	virtual long unsigned int GetDeletedPlatformsCount() override;
	//? get number of platforms which top() is bigger (or equal) then given coordinate player bottom()
	virtual long unsigned int GetAllBellowPlatformsCount(const int& actor_bottom) override;

	//* Manipulators

	virtual UniverseDot TraverseNewPlatforms(long unsigned int index) override;

	//@ API for level

	//* Setters

	void SetDynamicWorldConnection(IUniverse* new_world);
	void SetSceneConnection(ILocatableScene* new_scene);
	void SetPlayerColliderConnection(ICollidable* new_collider);

	//* Getters

	int GetPlatformsBottom() const;
	Point GetFirstPlatformTopCLocation() const;

	long unsigned int* GetNewPlatformsQuantity();

	//* Manipulators

	void Initialize();
	virtual void OnSpawnNewActor(const int& x, const int& y, const unsigned int& variance) override;
	void CheckForSpawnNewPlatforms(const int& player_y);
	//? Call it every frame to check if we can delete some platforms
	void Cleaner();
	void Tick(float delta_t);

	//* Render

	void Render();

	//@ API for collider

	virtual void OnTopOfPlatformMessaging(long unsigned int index) override;
	virtual void OnFlyingMessaging() override;

private:
	//@ Methods

	//* Let's prevent copying of the module
	WorldStatic(const WorldStatic&) = delete;
	WorldStatic& operator=(const WorldStatic&) = delete;

	//* Safe funstions to add new actor
	void OnAddNewActorToScene(IRelocatableActor* new_actor);
	void OnAddNewActorToPlayerCollider(RectangleCore* new_actor);

	//* Safe functions to remove front actor
	void OnRemoveFrontActorFromScene();
	void OnRemoveFrontActorFromPlayerCollider();

	//@ Members

	std::vector<std::unique_ptr<Image>> _platforms;
	std::vector<std::unique_ptr<Locator>> _platformsLocators;

	std::unique_ptr<RandomSpawn> _platformSpawner{ nullptr };

	//* Platforms hide module
	std::unique_ptr<Stealth> _stealth{ nullptr };

	//* Level connectors
	ILocatableScene* _scene{ nullptr };
	ICollidable* _playerCollider{ nullptr };

	//* Connection to the Dynamic World
	IUniverse* _dynamicWorld;

	//* Utilities: Screen
	const ScreenItem* _screen{ nullptr };

	//* Images path
	const char* _platformsPathCut{};
	const char* _platformsPathAdder{};

	const unsigned int _platformWidth{};
	const unsigned int _platformHeight{};

	//* Traversing
	long unsigned int _newPlatformsQuantity{};

	//* Debugging
	long unsigned int _overallDeleted{};	// TODO: serialize this and set after level reload

	const unsigned int _platformsQuantity{};

	//* Checker
	int _thresholdY{};
};
