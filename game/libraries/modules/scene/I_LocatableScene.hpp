#pragma once

//* Custom
//? Inheritance
//? Interfaces

//* C++ std

//* Forward declarations
class IRelocatableActor;
// using size_t = long unsigned int;

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
	virtual void OnAmmoMissed(long unsigned int) = 0;
	virtual void OnEnemyKilled(long unsigned int) = 0;
	virtual void OnAmmoDestroyed(long unsigned int) = 0;

	virtual void RelocateAll(int, int) = 0;
};
