//* Source header
#include "modules/scene/Scene.hpp"

//* Game
#include "modules/locator/I_RelocatableActor.hpp"

//* C++ std
#include <format>
#include <iostream>

void Scene::addPlayer(IRelocatableActor* actor) { _Player = actor; }

void Scene::addScoreBoardOrigin(IRelocatableActor* actor) { _ScoreBoardOrigin = actor; }

void Scene::addRelocatableActor(IRelocatableActor* actor) { _StaticActors.push_back(actor); }

void Scene::addRelocatableEnemy(IRelocatableActor* actor) { _Enemies.push_back(actor); }

void Scene::addRelocatableWormhole(IRelocatableActor* actor) { _Wormholes.push_back(actor); }

void Scene::addRelocatableAmmo(IRelocatableActor* actor) { _Ammos.push_back(actor); }

void Scene::removeRelocatableActorAtFront()
{
	if (!_StaticActors.empty())
	{
		_StaticActors.erase(_StaticActors.begin());
	}
}

void Scene::removeRelocatableEnemyAtFront()
{
	if (!_Enemies.empty())
	{
		_Enemies.erase(_Enemies.begin());
	}
}

void Scene::removeRelocatableWormholeAtFront()
{
	if (!_Wormholes.empty())
	{
		_Wormholes.erase(_Wormholes.begin());
	}
}

void Scene::removeRelocatableAmmoAtFront()
{
	if (!_Ammos.empty())
	{
		_Ammos.erase(_Ammos.begin());
	}
}

void Scene::onAmmoMissed(size_t index)
{
	if (!_Ammos.empty())
	{
		_Ammos.erase(_Ammos.begin() + index);
	}
}

void Scene::onEnemyKilled(size_t index)
{
	if (!_Enemies.empty())
	{
		_Enemies.erase(_Enemies.begin() + index);
	}
}

void Scene::onAmmoDestroyed(size_t index)
{
	if (!_Ammos.empty())
	{
		_Ammos.erase(_Ammos.begin() + index);
	}
}

void Scene::addBackground(IRelocatableActor* back) { _Background = back; }

void Scene::relocateAll(int delta_x, int delta_y)
{
	if (_Player)
	{
		_Player->relocate(delta_x, delta_y);
	}

	for (auto& Actor : _StaticActors)
	{
		Actor->relocate(delta_x, delta_y);
	}

	for (auto& Enemy : _Enemies)
	{
		Enemy->relocate(delta_x, delta_y);
	}

	for (auto& Wormhole : _Wormholes)
	{
		Wormhole->relocate(delta_x, delta_y);
	}

	if (_Background)
	{
		_Background->relocate(delta_x, delta_y);
	}

	for (auto& Ammo : _Ammos)
	{
		Ammo->relocate(delta_x, delta_y);
	}

	if (_ScoreBoardOrigin)
	{
		_ScoreBoardOrigin->relocate(delta_x, delta_y);
	}
}
