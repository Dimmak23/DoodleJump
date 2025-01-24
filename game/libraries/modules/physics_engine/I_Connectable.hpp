#pragma once

//* Custom
//? Inheritance
//? Interfaces

//* C++ std

//* Forward declarations
struct Point;

class IConnectable
{
public:
	//* Setters
	virtual void SetPreciseCoordinate(int, int) = 0;
	virtual void MoveByCamera(Point) = 0;
};
