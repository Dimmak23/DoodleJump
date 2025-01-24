#pragma once

//* Game
#include "level/world/interface/I_DynamicSpawn.hpp"
#include "level/world/interface/I_Universe.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class Image;
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

class WorldDynamic : public IUniverse, public IDynamicSpawn
{
	//* Custom types
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
	virtual UniverseDot TraverseNewPlatforms(size_t index) override;
	virtual void OnSpawnNewActor(const int& x, const int& y, const unsigned int& variance) override;

	//@ API for other worlds

	//* Getters

	//* Manipulators

	void OnSpawnNewEnemy(const int& x, const int& y, const unsigned int& variance);
	void OnSpawnNewWormhole(const int& x, const int& y, const unsigned int& variance);
	virtual void OnSpawnNewMoveableActor(const Point& begin, const Point& end) override;

	//@ API for level

	//* Setters

	void SetLevelConnection(IStoppable* new_level);
	void SetStaticWorldConnection(IUniverse* new_world);
	void SetSceneConnection(ILocatableScene* new_scene);
	void SetPlayerColliderConnection(ICollidable* new_collider);
	void SetPlayerBodyConnection(RectangleShape* character_body);
	void SetPlayerEngineConnection(IMechanics* character_engine);

	//* Manipulators

	void Initialize();
	void CheckForSpawnNewEnemies(size_t* new_platforms_quantity);
	void CheckForSpawnNewWormholes();
	void OnEnemyKilled(size_t index);	   //? Calling only Scene there
	void OnAmmoDestroyed(size_t index);	   //? Calling only Scene there
	//? Call it every frame to check if we can delete some platforms
	void Cleaner();

	void Tick(float delta_t);

	//* Graphics
	//! Maybe all renders should be const methods?
	void Render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	WorldDynamic(const WorldDynamic&) = delete;
	WorldDynamic& operator=(const WorldDynamic&) = delete;

	//* Safe funstions to add new actor
	void OnAddNewActorToScene(IRelocatableActor* new_actor);
	void OnAddNewActorToPlayerCollider(RectangleCore* new_actor);
	void OnAddNewWormholeToScene(IRelocatableActor* new_actor);
	void OnAddNewWormholeToPlayerCollider(RectangleCore* new_actor);
	void OnAddNewAmmoToScene(IRelocatableActor* new_actor);
	void OnAddNewAmmoToPlayerCollider(RectangleCore* new_actor, IMechanics* engine);

	//* Safe functions to remove front actor
	void OnRemoveFrontActorFromScene();
	void OnRemoveFrontActorFromPlayerCollider();
	void OnRemoveFrontWormholeFromScene();
	void OnRemoveFrontWormholeFromPlayerCollider();
	void OnRemoveAmmoMissed(size_t index);
	void OnRemoveFrontAmmoFromScene();
	void OnRemoveFrontAmmoFromPlayerCollider();

	void DoIndexTraversing(int& counter, RandomSpawn* spawner, SpawnCallBackType onSpawn,
						   ContainerPopulationCallBackType onSave);
	void OnSaveAvailablePlaces(const int);

	//@ Members

	//* Containers
	std::vector<std::unique_ptr<Image>> _enemies;
	std::vector<std::unique_ptr<Locator>> _enemyLocators;	 // TODO: find a way to use single locator for all
	std::vector<std::unique_ptr<Ammo>> _ammoTiles;
	std::vector<std::unique_ptr<Image>> _wormholes;
	std::vector<std::unique_ptr<Locator>> _wormholesLocators;	 // TODO: find a way to use single locator for all
	std::vector<std::unique_ptr<Wormhole>> _wormholesEngines;
	std::vector<int> _emptyPlatformsIndecies;

	std::unique_ptr<RandomSpawn> _enemySpawner{ nullptr };
	std::unique_ptr<RandomSpawn> _wormholeSpawner{ nullptr };

	//* Level connectors
	ILocatableScene* _scene{ nullptr };
	ICollidable* _playerCollider{ nullptr };
	RectangleShape* _playerBody{ nullptr };
	IMechanics* _playerEngine{ nullptr };

	//* Connection to the Static World
	IUniverse* _staticWorld;

	//* Parents
	const ScreenItem* _screen{ nullptr };
	IStoppable* _level{ nullptr };

	//* Images path
	const char* _enemiesPathCut{};
	const char* _enemiesPathAdder{};
	const char* _wormholePath{};
	const char* _ammoPath{};

	//* Checker
	int _thresholdY{};

	/*
	//* Debugging
	size_t _overallDeleted{};			  // TODO: serialize this and set after level reload
	size_t _overallDeletedAmmos{};		  // TODO: serialize this and set after level reload
	size_t _overallDeletedWormholes{};	  // TODO: serialize this and set after level reload
	 */
	const unsigned int _enemiesQuantity{};

	bool _bOneWormholeSpawned{ false };
};
