#pragma once

//* Game

//* C++ std

//* Forward declarations
class RectangleCore;
class IMechanics;

class ICollidable
{
public:
	virtual void AddBlockedActor(RectangleCore*) = 0;
	virtual void AddBlockedEnemy(RectangleCore*) = 0;
	virtual void AddBlockedWormhole(RectangleCore*) = 0;
	virtual void AddShooterAmmo(RectangleCore*, IMechanics*) = 0;
	virtual void RemoveBlockedActorAtFront() = 0;
	virtual void RemoveEnemyAtFront() = 0;
	virtual void RemoveWormholeAtFront() = 0;
	virtual void RemoveAmmoAtFront() = 0;
};
