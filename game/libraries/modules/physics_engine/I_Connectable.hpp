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
	virtual void setPreciseCoordinate(int, int) = 0;
	virtual void moveByCamera(Point) = 0;
};
