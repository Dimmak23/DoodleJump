#pragma once

//* Forward declarations
using size_t = long unsigned int;

class IStoppable
{
public:
	virtual void OnLevelStop() = 0;
	virtual void OnEnemyKilled(size_t) = 0;
	virtual void OnAmmoDestroyed(size_t) = 0;
};
