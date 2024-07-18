#pragma once

//* Forward declarations
using size_t = unsigned long long int;

class IStoppable
{
public:
	virtual void onLevelStop() = 0;
	virtual void onEnemyKilled(size_t) = 0;
	virtual void onAmmoDestroyed(size_t) = 0;
};
