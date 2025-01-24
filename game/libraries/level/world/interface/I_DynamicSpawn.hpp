#pragma once

//* Forward declarations
struct Point;

class IDynamicSpawn
{
public:
	virtual void OnSpawnNewMoveableActor(const Point&, const Point&) = 0;
};
