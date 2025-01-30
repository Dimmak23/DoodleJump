#pragma once

//* Forward declarations
// using size_t = long unsigned int;

class IStoppable
{
public:
	virtual void OnLevelStop() = 0;
	virtual void OnEnemyKilled(long unsigned int) = 0;
	virtual void OnAmmoDestroyed(long unsigned int) = 0;
};
