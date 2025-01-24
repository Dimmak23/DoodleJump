//* Source header
#include "modules/scene/Scene.hpp"

//* Game
#include "modules/locator/I_RelocatableActor.hpp"

//* C++ std
#include <format>
#include <iostream>

void Scene::AddPlayer(IRelocatableActor* actor) { _player = actor; }

void Scene::AddScoreBoardOrigin(IRelocatableActor* actor) { _scoreBoardOrigin = actor; }

void Scene::AddRelocatableActor(IRelocatableActor* actor) { _staticActors.push_back(actor); }

void Scene::AddRelocatableEnemy(IRelocatableActor* actor) { _enemies.push_back(actor); }

void Scene::AddRelocatableWormhole(IRelocatableActor* actor) { _wormholes.push_back(actor); }

void Scene::AddRelocatableAmmo(IRelocatableActor* actor) { _ammos.push_back(actor); }

void Scene::RemoveRelocatableActorAtFront()
{
	if (!_staticActors.empty())
	{
		_staticActors.erase(_staticActors.begin());
	}
}

void Scene::RemoveRelocatableEnemyAtFront()
{
	if (!_enemies.empty())
	{
		_enemies.erase(_enemies.begin());
	}
}

void Scene::RemoveRelocatableWormholeAtFront()
{
	if (!_wormholes.empty())
	{
		_wormholes.erase(_wormholes.begin());
	}
}

void Scene::RemoveRelocatableAmmoAtFront()
{
	if (!_ammos.empty())
	{
		_ammos.erase(_ammos.begin());
	}
}

void Scene::OnAmmoMissed(size_t index)
{
	if (!_ammos.empty())
	{
		_ammos.erase(_ammos.begin() + index);
	}
}

void Scene::OnEnemyKilled(size_t index)
{
	if (!_enemies.empty())
	{
		_enemies.erase(_enemies.begin() + index);
	}
}

void Scene::OnAmmoDestroyed(size_t index)
{
	if (!_ammos.empty())
	{
		_ammos.erase(_ammos.begin() + index);
	}
}

void Scene::AddBackground(IRelocatableActor* back) { _background = back; }

void Scene::RelocateAll(int delta_x, int delta_y)
{
	if (_player)
	{
		_player->Relocate(delta_x, delta_y);
	}

	for (auto& Actor : _staticActors)
	{
		Actor->Relocate(delta_x, delta_y);
	}

	for (auto& Enemy : _enemies)
	{
		Enemy->Relocate(delta_x, delta_y);
	}

	for (auto& Wormhole : _wormholes)
	{
		Wormhole->Relocate(delta_x, delta_y);
	}

	if (_background)
	{
		_background->Relocate(delta_x, delta_y);
	}

	for (auto& Ammo : _ammos)
	{
		Ammo->Relocate(delta_x, delta_y);
	}

	if (_scoreBoardOrigin)
	{
		_scoreBoardOrigin->Relocate(delta_x, delta_y);
	}
}
