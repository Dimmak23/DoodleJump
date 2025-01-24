//* Source header
#include "modules/gun/Gun.hpp"

//* SDL2: Connector
#include "SDL2_connector/Loger.hpp"

//* Game
#include "actor/interface/I_Shooter.hpp"
#include "level/world/interface/I_DynamicSpawn.hpp"

//* C++ std
#include <format>
#include <iostream>

Gun::Gun() {}

Gun::~Gun() {}

void Gun::SetOwner(IShooter* owner) { _owningActor = owner; }

void Gun::SetDynamicWorldConnection(IDynamicSpawn* dynamic_world) { _dynamicWorld = dynamic_world; }

void Gun::SetOnSpawnCallBack(SpawnCallBackType function) { _spawnAmmo = function; }

void Gun::SetShootingRate(const float& new_rate) { _shootingRate = new_rate; }

void Gun::Shoot(const Point& end, const float& delta_t)
{
	if (_currentDelay <= 1.f)
	{
		//? Actually shoot
		OnSpawnAmmo(end);
		//? Reset timer
		_currentDelay = _shootingRate;
	}
	else
	{
		_currentDelay -= delta_t;
	}
}

void Gun::Reload(float delta_t)
{
	_currentDelay -= delta_t;
	if (_currentDelay <= 1.f)
	{
		_currentDelay = 0.f;
	}
}

void Gun::OnSpawnAmmo(const Point& end)
{
	if (_dynamicWorld && _spawnAmmo && _owningActor)
	{
		//? Animate character
		_owningActor->SetShootingSide(end);
		LogLine("_SpawnAmmo: ", _spawnAmmo);	//! why this 0xFFFFFF...F?
		//? Doing re-calculations of spawn point only when it accessed
		(_dynamicWorld->*_spawnAmmo)(*(_owningActor->GetSpawnPoint()), end);
	}
}
