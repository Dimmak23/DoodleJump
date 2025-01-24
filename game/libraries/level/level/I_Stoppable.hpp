#pragma once

//* Forward declarations
using size_t = unsigned long long int;

class IStoppable
{
public:
	virtual void OnLevelStop() = 0;
	virtual void OnEnemyKilled(size_t) = 0;
	virtual void OnAmmoDestroyed(size_t) = 0;
};
