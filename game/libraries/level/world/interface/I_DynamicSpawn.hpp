#pragma once

//* Forward declarations
struct Point;

class IDynamicSpawn
{
public:
	virtual void onSpawnNewMoveableActor(const Point&, const Point&) = 0;
};
