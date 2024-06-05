#pragma once

//* Custom
//? Inheritance
//? Interfaces

//* C++ std

//* Forward declarations
class RectangleCore;
class IMechanics;

class ICollidable
{
public:
	virtual void addBlockedActor(RectangleCore*) = 0;
	virtual void addBlockedEnemy(RectangleCore*) = 0;
	virtual void addBlockedWormhole(RectangleCore*) = 0;
	virtual void addShooterAmmo(RectangleCore*, IMechanics*) = 0;
	virtual void removeBlockedActorAtFront() = 0;
	virtual void removeEnemyAtFront() = 0;
	virtual void removeWormholeAtFront() = 0;
	virtual void removeAmmoAtFront() = 0;
};
