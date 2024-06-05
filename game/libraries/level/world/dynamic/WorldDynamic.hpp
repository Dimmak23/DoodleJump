#pragma once

#pragma once

//* Custom headers
//? Modules
//? Inheritance
//? Interfaces
#include "I_DynamicSpawn.hpp"
#include "I_Universe.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class SpriteWrapper;
class Locator;
class ILocatableScene;
class ICollidable;
class RandomSpawn;
class IRelocatableActor;
class RectangleCore;
struct ScreenItem;
struct Point;
class Ammo;
class IMechanics;
class Wormhole;
class RectangleShape;
class IStoppable;

//* Custom types

class WorldDynamic : public IUniverse, public IDynamicSpawn
{
	using SpawnCallBackType = void (WorldDynamic::*)(const int&, const int&, const unsigned int&);
	using ContainerPopulationCallBackType = void (WorldDynamic::*)(const int);

public:
	WorldDynamic(const ScreenItem* parent_screen,		 //
				 const char* enemies_path_cut,			 //
				 const char* enemies_path_adder,		 //
				 const unsigned int enemies_quantity,	 //
				 const char* wormhole_path,				 //
				 const char* ammo_path);
	virtual ~WorldDynamic();

	//* Empty
	// TODO: find a way to get rid of empty implementations from interfaces
	virtual UniverseDot traverseNewPlatforms(size_t index) override;
	virtual void onSpawnNewActor(const int& x, const int& y, const unsigned int& variance) override;

	//@ API for other worlds

	//* Getters

	//* Manipulators

	void onSpawnNewEnemy(const int& x, const int& y, const unsigned int& variance);
	void onSpawnNewWormhole(const int& x, const int& y, const unsigned int& variance);
	virtual void onSpawnNewMoveableActor(const Point& begin, const Point& end) override;

	//@ API for level

	//* Setters

	void setLevelConnection(IStoppable* new_level);
	void setStaticWorldConnection(IUniverse* new_world);
	void setSceneConnection(ILocatableScene* new_scene);
	void setPlayerColliderConnection(ICollidable* new_collider);
	void setPlayerBodyConnection(RectangleShape* character_body);
	void setPlayerEngineConnection(IMechanics* character_engine);

	//* Manipulators

	void initialize();
	void checkForSpawnNewEnemies(size_t* new_platforms_quantity);
	void checkForSpawnNewWormholes();
	void onEnemyKilled(size_t index);	   //? Calling only Scene there
	void onAmmoDestroyed(size_t index);	   //? Calling only Scene there
	//? Call it every frame to check if we can delete some platforms
	void cleaner();

	void tick(float delta_t);

	//* Graphics
	//! Maybe all renders should be const methods
	void render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	WorldDynamic(const WorldDynamic&) = delete;
	WorldDynamic& operator=(const WorldDynamic&) = delete;

	//* Safe funstions to add new actor
	void onAddNewActorToScene(IRelocatableActor* new_actor);
	void onAddNewActorToPlayerCollider(RectangleCore* new_actor);
	void onAddNewWormholeToScene(IRelocatableActor* new_actor);
	void onAddNewWormholeToPlayerCollider(RectangleCore* new_actor);
	void onAddNewAmmoToScene(IRelocatableActor* new_actor);
	void onAddNewAmmoToPlayerCollider(RectangleCore* new_actor, IMechanics* engine);

	//* Safe functions to remove front actor
	void onRemoveFrontActorFromScene();
	void onRemoveFrontActorFromPlayerCollider();
	void onRemoveFrontWormholeFromScene();
	void onRemoveFrontWormholeFromPlayerCollider();
	void onRemoveAmmoMissed(size_t index);
	void onRemoveFrontAmmoFromScene();
	void onRemoveFrontAmmoFromPlayerCollider();

	void doIndexTraversing(int& counter, RandomSpawn* spawner, SpawnCallBackType onSpawn,
						   ContainerPopulationCallBackType onSave);
	void onSaveAvailablePlaces(const int);

	//@ Members

	//* Containers
	std::vector<std::unique_ptr<SpriteWrapper>> _Enemies;
	std::vector<std::unique_ptr<Locator>> _EnemyLocators;	 // TODO: find a way to use single locator for all
	std::vector<std::unique_ptr<Ammo>> _AmmoTiles;
	std::vector<std::unique_ptr<SpriteWrapper>> _Wormholes;
	std::vector<std::unique_ptr<Locator>> _WormholesLocators;	 // TODO: find a way to use single locator for all
	std::vector<std::unique_ptr<Wormhole>> _WormholesEngines;
	std::vector<int> _EmptyPlatformsIndecies;

	std::unique_ptr<RandomSpawn> _EnemySpawner{ nullptr };
	std::unique_ptr<RandomSpawn> _WormholeSpawner{ nullptr };

	//* Level connectors
	ILocatableScene* _Scene{ nullptr };
	ICollidable* _PlayerCollider{ nullptr };
	RectangleShape* _PlayerBody{ nullptr };
	IMechanics* _PlayerEngine{ nullptr };

	//* Connection to the Static World
	IUniverse* _StaticWorld;

	//* Parents
	const ScreenItem* _Screen{ nullptr };
	IStoppable* _Level{ nullptr };

	//* Checker
	int _ThresholdY{};

	//* Debugging
	size_t _OverallDeleted{};			  // TODO: serialize this and set after level reload
	size_t _OverallDeletedAmmos{};		  // TODO: serialize this and set after level reload
	size_t _OverallDeletedWormholes{};	  // TODO: serialize this and set after level reload
	bool _bOneWormholeSpawned{ false };

	//* Images path
	const char* _EnemiesPathCut{};
	const char* _EnemiesPathAdder{};
	const unsigned int _EnemiesQuantity{};
	const char* _WormholePath{};
	const char* _AmmoPath{};
};
