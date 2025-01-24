#pragma once

//* Custom
//? Inheritance
//? Interfaces

//* C++ std

//* Forward declarations
class IRelocatableActor;
using size_t = unsigned long long int;

class ILocatableScene
{
public:
	virtual void AddRelocatableActor(IRelocatableActor*) = 0;
	virtual void AddRelocatableEnemy(IRelocatableActor*) = 0;
	virtual void AddRelocatableWormhole(IRelocatableActor*) = 0;
	virtual void AddRelocatableAmmo(IRelocatableActor*) = 0;
	virtual void RemoveRelocatableActorAtFront() = 0;
	virtual void RemoveRelocatableEnemyAtFront() = 0;
	virtual void RemoveRelocatableWormholeAtFront() = 0;
	virtual void RemoveRelocatableAmmoAtFront() = 0;
	virtual void OnAmmoMissed(size_t) = 0;
	virtual void OnEnemyKilled(size_t) = 0;
	virtual void OnAmmoDestroyed(size_t) = 0;

	virtual void RelocateAll(int, int) = 0;
};
