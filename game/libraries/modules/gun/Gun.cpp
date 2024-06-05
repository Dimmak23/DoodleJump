
//* Source header
#include "Gun.hpp"

//* C++ std
#include <format>
#include <iostream>

//* Custom
//? Inheritance
#include "I_DynamicSpawn.hpp"
#include "I_Shooter.hpp"
//? Interfaces
//? Modules
//? Wrappers
//? Worlds
//? Utilities

Gun::Gun() {}

Gun::~Gun() {}

void Gun::setOwner(IShooter* owner) { _OwningActor = owner; }

void Gun::setDynamicWorldConnection(IDynamicSpawn* dynamic_world) { _DynamicWorld = dynamic_world; }

void Gun::setOnSpawnCallBack(SpawnCallBackType function) { _SpawnAmmo = function; }

void Gun::setShootingRate(const float& new_rate) { _ShootingRate = new_rate; }

void Gun::shoot(const Point& end, const float& delta_t)
{
	if (_CurrentDelay <= 1.f)
	{
		// std::cout << "shoot(const Point& end, const float& delta_t)\n";
		//? Actually shoot
		onSpawnAmmo(end);
		//? Reset timer
		_CurrentDelay = _ShootingRate;
	}
	else
	{
		_CurrentDelay -= delta_t;
	}
}

void Gun::reload(float delta_t)
{
	_CurrentDelay -= delta_t;
	if (_CurrentDelay <= 1.f)
	{
		_CurrentDelay = 0.f;
	}
}

void Gun::onSpawnAmmo(const Point& end)
{
	if (_DynamicWorld && _SpawnAmmo && _OwningActor)
	{
		//? Animate character
		_OwningActor->setShootingSide(end);
		//? Doing re-calculations of spawn point only when it accessed
		(_DynamicWorld->*_SpawnAmmo)(*(_OwningActor->getSpawnPoint()), end);
	}
}
