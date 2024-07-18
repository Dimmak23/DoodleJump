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

	void setOwner(IShooter* owner);
	void setDynamicWorldConnection(IDynamicSpawn* dynamic_world);
	void setOnSpawnCallBack(SpawnCallBackType function);
	void setShootingRate(const float& new_rate);

	//* Manipulators

	void shoot(const Point& end, const float& delta_t);
	void reload(float delta_t);

private:
	//@ Methods

	//* Let's prevent copying of the module
	Gun(const Gun&) = delete;
	Gun& operator=(const Gun&) = delete;

	//* Safe callbacks
	void onSpawnAmmo(const Point&);

	//@ Members

	//* Timers
	float _ShootingRate{ 200.f };	 //? in milliseconds
	float _CurrentDelay{ 0.f };

	//* World
	IDynamicSpawn* _DynamicWorld{ nullptr };

	//* Player access
	IShooter* _OwningActor{ nullptr };

	//* Spawn callback
	SpawnCallBackType _SpawnAmmo{ nullptr };
};
