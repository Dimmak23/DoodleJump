#pragma once

//* Forward declarations
struct Point;
class IDynamicSpawn;
using SpawnCallBackType = void (IDynamicSpawn::*)(const Point&, const Point&);
class IShooter;

class Gun
{
public:
	Gun();
	~Gun();

	//@ API

	//* Setters

	void SetOwner(IShooter* owner);
	void SetDynamicWorldConnection(IDynamicSpawn* dynamic_world);
	void SetOnSpawnCallBack(SpawnCallBackType function);
	void SetShootingRate(const float& new_rate);

	//* Manipulators

	void Shoot(const Point& end, const float& delta_t);
	void Reload(float delta_t);

private:
	//@ Methods

	//* Let's prevent copying of the module
	Gun(const Gun&) = delete;
	Gun& operator=(const Gun&) = delete;

	//* Safe callbacks
	void OnSpawnAmmo(const Point&);

	//@ Members

	//* World
	IDynamicSpawn* _dynamicWorld{ nullptr };

	//* Player access
	IShooter* _owningActor{ nullptr };

	//* Spawn callback
	SpawnCallBackType _spawnAmmo{ nullptr };

	//* Timers
	float _shootingRate{ 200.f };	 //? in milliseconds
	float _currentDelay{ 0.f };
};
