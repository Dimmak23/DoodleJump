#pragma once

//* Custom
//? Inheritance
//? Interfaces

//* C++ std

//* Forward declarations
struct Point;
struct Deltas;

struct LinearSpeed
{
	double _Vx, _Vy;
};

class IMechanics
{
public:
	//* Movers
	virtual void move() = 0;
	virtual void move(Point) = 0;
	//* Collisions
	virtual void setOnTopOfAnyPlatform(bool) = 0;
	virtual bool getOnPlatform() const = 0;
	virtual void setOnTopOfFrame(bool) = 0;
	virtual void setIsJumping(bool) = 0;
	virtual void setIsFalling(bool) = 0;
	virtual bool getIsJumping() const = 0;
	virtual bool getIsFalling() const = 0;
	virtual void setAirFrictionY(double) = 0;
	//* Setters
	virtual void setConstantVelocity(double, double) = 0;
	virtual void setCoordinate(int, int) = 0;
	//* Getters
	virtual LinearSpeed getLinearSpeed() = 0;
	//* Manipulators
	virtual void accelerateY(float) = 0;
	virtual void speedUpY(float) = 0;
	virtual void addForceY(float) = 0;
	virtual void clearAccelerationY() = 0;
	virtual void clearForceY() = 0;
	virtual void constantSpeedUpY(float, float) = 0;
	virtual void enableGravity(bool) = 0;
};
