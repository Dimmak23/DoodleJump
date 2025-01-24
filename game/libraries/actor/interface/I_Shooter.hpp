#pragma once

//* Forward declarations
struct Point;

class IShooter
{
public:
	virtual void SetShootingSide(const Point&) = 0;
	virtual const Point* GetSpawnPoint() = 0;
};
