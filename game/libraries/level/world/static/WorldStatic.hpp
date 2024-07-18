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
@ 		setSceneConnection(new_scene), setPlayerColliderConnection(new_collider).
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
	virtual size_t getDeletedPlatformsCount() override;
	//? get number of platforms which top() is bigger (or equal) then given coordinate player bottom()
	virtual size_t getAllBellowPlatformsCount(const int& actor_bottom) override;

	//* Manipulators

	virtual UniverseDot traverseNewPlatforms(size_t index) override;

	//@ API for level

	//* Setters

	void setDynamicWorldConnection(IUniverse* new_world);
	void setSceneConnection(ILocatableScene* new_scene);
	void setPlayerColliderConnection(ICollidable* new_collider);

	//* Getters

	int getPlatformsBottom() const;
	Point getFirstPlatformTopCLocation() const;

	size_t* getNewPlatformsQuantity();

	//* Manipulators

	void initialize();
	virtual void onSpawnNewActor(const int& x, const int& y, const unsigned int& variance) override;
	void checkForSpawnNewPlatforms(const int& player_y);
	//? Call it every frame to check if we can delete some platforms
	void cleaner();
	void tick(float delta_t);

	//* Render

	void render();

	//@ API for collider

	virtual void onTopOfPlatformMessaging(size_t index) override;
	virtual void onFlyingMessaging() override;

private:
	//@ Methods

	//* Let's prevent copying of the module
	WorldStatic(const WorldStatic&) = delete;
	WorldStatic& operator=(const WorldStatic&) = delete;

	//* Safe funstions to add new actor
	void onAddNewActorToScene(IRelocatableActor* new_actor);
	void onAddNewActorToPlayerCollider(RectangleCore* new_actor);

	//* Safe functions to remove front actor
	void onRemoveFrontActorFromScene();
	void onRemoveFrontActorFromPlayerCollider();

	//@ Members

	const unsigned int _PlatformWidth{};
	const unsigned int _PlatformHeight{};
	std::vector<std::unique_ptr<Image>> _Platforms;
	std::vector<std::unique_ptr<Locator>> _PlatformsLocators;

	std::unique_ptr<RandomSpawn> _PlatformSpawner{ nullptr };

	//* Level connectors
	ILocatableScene* _Scene{ nullptr };
	ICollidable* _PlayerCollider{ nullptr };

	//* Connection to the Dynamic World
	IUniverse* _DynamicWorld;

	//* Platforms hide module
	std::unique_ptr<Stealth> _Stealth{ nullptr };

	//* Checker
	int _ThresholdY{};

	//* Traversing
	size_t _NewPlatformsQuantity{};

	//* Utilities: Screen
	const ScreenItem* _Screen{ nullptr };

	//* Debugging
	size_t _OverallDeleted{};	 // TODO: serialize this and set after level reload

	//* Images path
	const char* _PlatformsPathCut{};
	const char* _PlatformsPathAdder{};
	const unsigned int _PlatformsQuantity{};
};
