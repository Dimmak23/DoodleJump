#pragma once

//* Game

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
	virtual void Move() = 0;
	virtual void Move(Point) = 0;
	//* Collisions
	virtual void SetOnTopOfAnyPlatform(bool) = 0;
	virtual bool GetOnPlatform() const = 0;
	virtual void SetOnTopOfFrame(bool) = 0;
	virtual void SetIsJumping(bool) = 0;
	virtual void SetIsFalling(bool) = 0;
	virtual bool GetIsJumping() const = 0;
	virtual bool GetIsFalling() const = 0;
	virtual void SetAirFrictionY(double) = 0;
	//* Setters
	virtual void SetConstantVelocity(double, double) = 0;
	virtual void SetCoordinate(int, int) = 0;
	//* Getters
	virtual LinearSpeed GetLinearSpeed() = 0;
	//* Manipulators
	virtual void AccelerateY(float) = 0;
	virtual void SpeedUpY(float) = 0;
	virtual void AddForceY(float) = 0;
	virtual void ClearAccelerationY() = 0;
	virtual void ClearForceY() = 0;
	virtual void ConstantSpeedUpY(float, float) = 0;
	virtual void EnableGravity(bool) = 0;
};
