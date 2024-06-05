#pragma once

//* Forward declarations
struct Point;

class IShooter
{
public:
	virtual void setShootingSide(const Point&) = 0;
	virtual const Point* getSpawnPoint() = 0;
};
