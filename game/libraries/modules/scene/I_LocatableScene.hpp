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
	virtual void addRelocatableActor(IRelocatableActor*) = 0;
	virtual void addRelocatableEnemy(IRelocatableActor*) = 0;
	virtual void addRelocatableWormhole(IRelocatableActor*) = 0;
	virtual void addRelocatableAmmo(IRelocatableActor*) = 0;
	virtual void removeRelocatableActorAtFront() = 0;
	virtual void removeRelocatableEnemyAtFront() = 0;
	virtual void removeRelocatableWormholeAtFront() = 0;
	virtual void removeRelocatableAmmoAtFront() = 0;
	virtual void onAmmoMissed(size_t) = 0;
	virtual void onEnemyKilled(size_t) = 0;
	virtual void onAmmoDestroyed(size_t) = 0;

	virtual void relocateAll(int, int) = 0;
};
