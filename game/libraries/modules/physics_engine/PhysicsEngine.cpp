//* Source header
#include "modules/physics_engine/PhysicsEngine.hpp"

//* Game
#include "rectangle/RectangleShape.hpp"

//* C++ std
#include <algorithm>
#include <format>
#include <iostream>

PhysicsEngine::PhysicsEngine(RectangleShape* rectangle) /* 	   //
	 : _hostBody(rectangle->getBody()), _hostTheta(rectangle->getAngle()) */
{
	// std::cout << "Constructing PhysicsEngine...\n";
	resetBody(rectangle->getBody());
	_hostTheta = rectangle->getAngle();

	// std::cout << "engine... x: " << _hostBody->x << ", y: " << _hostBody->y << '\n';
	// std::cout << "Constructed PhysicsEngine size of: " << sizeof(*this) << '\n';
}

PhysicsEngine::~PhysicsEngine()
{
	// delete _hostBody;	 //! can't do this, it's already deleted
	// delete _hostTheta;
	_hostBody = nullptr;
	_hostTheta = nullptr;
	// std::cout << "Deleted PhysicsEngine...\n";
}

void PhysicsEngine::setPreciseCoordinate(int x, int y)
{
	if (static_cast<int>(_X) != x)
	{
		_X = x;
	}
	if (static_cast<int>(_Y) != y)
	{
		_Y = y;
	}
}

void PhysicsEngine::setCoordinate(int x, int y) { setPreciseCoordinate(x, y); }

bool PhysicsEngine::speedUpOmega(double wSource, double wDestination, float timeDuration, float deltaTime)
{
	if (__SpentTime < timeDuration)
	{
		_Alpha = (wDestination - wSource) / double(timeDuration);
		__SpentTime += deltaTime;
		return false;
	}
	else
	{
		_Alpha = 0;
		__SpentTime = 0;
		return true;
	}
}

void PhysicsEngine::setDeltaTheta(double deltaTh) { _deltaTheta = deltaTh; }

void PhysicsEngine::clearDeltaTheta() { _deltaTheta *= 0; }

void PhysicsEngine::setVelocity(double velocity_x, double velocity_y)
{
	_VelocityX = velocity_x;
	_VelocityY = velocity_y;
}

void PhysicsEngine::setConstantVelocity(double velocity_x, double velocity_y)
{
	_ConstVelocityX = velocity_x;
	_ConstVelocityY = velocity_y;
}

void PhysicsEngine::clearVelocity()
{
	_VelocityX *= 0;
	_VelocityY *= 0;
}

void PhysicsEngine::resetBody(Rectangle* body)
{
	// std::cout << "resetBody(Rectangle* body)...\n";
	_hostBody = body;
	_X = _hostBody->x;
	_Y = _hostBody->y;
	// std::cout << "_X: " << _X << '\n';
	// std::cout << "_Y: " << _Y << '\n';
}

void PhysicsEngine::setMass(float NewMass) { _Mass = NewMass; }

void PhysicsEngine::setSurfaceFriction(double new_surface_friction) { _SurfaceFriction = new_surface_friction; }

void PhysicsEngine::setAirFrictionY(double new_air_friction) { _AirFrictionY = new_air_friction; }

void PhysicsEngine::setOnTopOfFrame(bool NewValue) { _bOnFrame = NewValue; }

void PhysicsEngine::setOnTopOfAnyPlatform(bool NewValue)
{
	// if (!NewValue)
	// {
	// 	std::cout << "Set to false...\n";
	// }
	// else
	// {
	// 	std::cout << "Set to true...\n";
	// }
	_bOnPlatform = NewValue;
}

void PhysicsEngine::setIsJumping(bool new_state)
{
	// if (!new_state)
	// {
	// 	std::cout << "Set to false...\n";
	// }
	// else
	// {
	// 	std::cout << "Set to true...\n";
	// }
	_bIsJumping = new_state;
}

void PhysicsEngine::setIsFalling(bool new_state) { _bIsFalling = new_state; }

void PhysicsEngine::setOmega(double omega) { _Omega = omega; }

void PhysicsEngine::clearOmega() { _Omega *= 0; }

void PhysicsEngine::clearDeltaOmega() { _deltaOmega *= 0; }

void PhysicsEngine::clearTimer() { __SpentTime = 0; }

void PhysicsEngine::updateOmega(float deltaT)
{
	_deltaOmega = _Alpha * double(deltaT);
	_Omega += _deltaOmega;
}

void PhysicsEngine::updateTheta(float deltaT) { _deltaTheta = _Omega * double(deltaT); }

void PhysicsEngine::rotate()
{
	*_hostTheta += _deltaTheta;
	if (*_hostTheta >= 360) *_hostTheta -= 360;
}

void PhysicsEngine::accelerateX(float deltaAccelerationX)
{
	// _AccelerationX += deltaAccelerationX;
	_deltaAccelerationX += deltaAccelerationX;
	// _deltaAccelerationX = std::clamp(_deltaAccelerationX, -10.f, 10.f);
	// _deltaAccelerationX = deltaAccelerationX;
}

void PhysicsEngine::accelerateY(float deltaAccelerationY)
{
	//_AccelerationY += deltaAccelerationY;
	_deltaAccelerationY += deltaAccelerationY;
	// _deltaAccelerationY = std::clamp(_deltaAccelerationY, -10.f, 10.f);
	// _deltaAccelerationY = deltaAccelerationY;
}

void PhysicsEngine::clearAccelerationX() { _deltaAccelerationX *= 0.98f; }

void PhysicsEngine::clearAccelerationY() { _deltaAccelerationY *= 0.98f; }

void PhysicsEngine::clearAll()
{
	_forceX = 0;
	_forceY = 0;
	_deltaVelocityX = 0;
	_deltaVelocityY = 0;
	_VelocityX = 0;
	_VelocityY = 0;
	_deltaAccelerationX = 0;
	_deltaAccelerationY = 0;
	_AccelerationX = 0;
	_AccelerationY = 0;
	_deltaX = 0;
	_deltaY = 0;
}

void PhysicsEngine::speedUpX(float deltaVelocityX) { _VelocityX += double(deltaVelocityX); }

void PhysicsEngine::speedUpY(float deltaVelocityY) { _VelocityY += double(deltaVelocityY); }

Deltas PhysicsEngine::constantSpeedUp(float delta_t, float delta_x, float delta_y)
{
	//? How much frames would need to move to the specific coordinate at X
	double _FramesX;
	_FramesX = (delta_x != 0) ? std::abs(double(delta_x) / (_ConstVelocityX * double(delta_t))) : 0;
	//? How much frames would need to move to the specific coordinate at Y
	double _FramesY;
	_FramesY = (delta_y != 0) ? std::abs(double(delta_y) / (_ConstVelocityY * double(delta_t))) : 0;

	_deltaX = (static_cast<int>(_FramesX) != 0) ? (delta_x / _FramesX) : 0;
	_deltaY = (static_cast<int>(_FramesY) != 0) ? (delta_y / _FramesY) : 0;

	return { _deltaX, _deltaY };
}

void PhysicsEngine::constantSpeedUpY(float delta_t, float delta_y)
{
	auto Unused = constantSpeedUp(delta_t, 0.f, delta_y);
	Unused;	   //!
}

void PhysicsEngine::updateImpulse(float deltaT)
{
	_forceX *= _ImpulseLoss;
	_VelocityX = _forceX * deltaT / _Mass;
	_VelocityX += _AccelerationX * double(deltaT);
	// _VelocityX *= _FrictionX;
	// _VelocityX *= double(1 - deltaT);

	_forceY *= _ImpulseLoss;
	_VelocityY = _forceY * deltaT / _Mass;
	_VelocityY += _AccelerationY * double(deltaT);
	_VelocityY += _bIsGravityEnabled * (std::abs(_VelocityY) + _GRAVITY) * double(deltaT) * _AirFrictionY;
	// std::cout << std::format("gravity: {}.\n",
	// 						 _bIsEnabled * (std::abs(_VelocityY) + _GRAVITY) * double(deltaT) * _AirFrictionY);
	// std::cout << std::format("_VelocityX: {}, _VelocityY: {}.\n", _VelocityX, _VelocityY);
	// _VelocityY *= _FrictionY;
	// _VelocityY *= double(1 - deltaT);
}

void PhysicsEngine::updatePositions(float deltaT)
{
	_deltaX = _VelocityX * double(deltaT);
	_deltaY = _VelocityY * double(deltaT);
	// std::cout << std::format("_deltaX: {}, _deltaY: {}.\n", _deltaX, _deltaY);
}

void PhysicsEngine::updateVelocities(float deltaT)
{
	// _deltaVelocityX = _AccelerationX * double(deltaT);
	// _VelocityX += _deltaVelocityX;
	_VelocityX += _AccelerationX * double(deltaT);

	_VelocityX += _forceX * deltaT / _Mass;

	_VelocityX *= _SurfaceFriction;

	_VelocityX = std::clamp(_VelocityX, _MinVelocityX, _MaxVelocityX);

	// _deltaVelocityY = _AccelerationY * double(deltaT);
	// _VelocityY += _deltaVelocityY;
	_VelocityY += _AccelerationY * double(deltaT);

	// std::cout << "deltaVelY: "
	// 		  << !(_onPlatform || _onFrame || !_bIsEnabled) * _VelocityY * double(deltaT) * _AirFrictionY << '\n';
	// TODO: maybe increase gravity when camera is moving
	_VelocityY += !(_bOnPlatform || _bOnFrame || !_bIsGravityEnabled) * _GRAVITY * double(deltaT) * _AirFrictionY;
	_VelocityY += !_bIsGravityEnabled * std::abs(_VelocityY) * double(deltaT) * _AirFrictionY;
	// std::cout << "grVel: " << !_bIsGravityEnabled * std::abs(_VelocityY) * double(deltaT) * _AirFrictionY << '\n';
	// std::cout << "gravity: "
	// 		  << !(_bOnPlatform || _bOnFrame || !_bIsGravityEnabled) * _GRAVITY * double(deltaT) * _AirFrictionY
	// 		  << '\n';

	_VelocityY += _forceY * deltaT / _Mass;

	_VelocityY *= _SurfaceFriction;

	_VelocityY = std::clamp(_VelocityY, _MinVelocityY, _MaxVelocityY);

	// std::cout << std::format("_VelocityX: {}, _VelocityY: {}.\n", _VelocityX, _VelocityY);
}

void PhysicsEngine::updateAmmoAccelerations(float deltaT)
{
	// TODO: friction*delta, and added to acceleration

	//  _deltaAccelerationX -= _FrictionX * _forceX / _Mass;
	auto SurfaceDecreaser = _SurfaceFriction * deltaT * 100;
	auto AccDecreaser = _AccelerationFriction * deltaT * 100;

	//! back this
	// _AccelerationX += _deltaAccelerationX;
	// _AccelerationX *= _FrictionX;
	_deltaAccelerationX *= SurfaceDecreaser;
	_AccelerationX += _deltaAccelerationX;
	_AccelerationX *= AccDecreaser;

	// _deltaAccelerationY -= _FrictionY * _forceY / _Mass;

	//! back this
	// _AccelerationY += _deltaAccelerationY;
	// _AccelerationY *= _FrictionY;
	_deltaAccelerationY *= SurfaceDecreaser;
	_AccelerationY += _deltaAccelerationY;
	_AccelerationY *= AccDecreaser;

	_AccelerationX = std::clamp(_AccelerationX, _MinAccelerationX, _MaxAccelerationX);
	_AccelerationY = std::clamp(_AccelerationY, _MinAccelerationY, _MaxAccelerationY);

	if ((_AccelerationX >= -0.01) && (_AccelerationX <= 0.01))
	{
		_AccelerationX = 0;
	}
	else
	{
		// std::cout << std::format("_AccelerationX: {}.\n", _AccelerationX);
	}
	if ((_AccelerationY >= -0.01) && (_AccelerationY <= 0.01))
	{
		_AccelerationY = 0;
	}
	else
	{
		// std::cout << std::format("_AccelerationY: {}.\n", _AccelerationY);
	}
}

void PhysicsEngine::updateAccelerations(float deltaT)
{
	// TODO: maybe apply force: dA = dF/M

	_AccelerationX += _deltaAccelerationX;
	_AccelerationX *= _SurfaceFriction;

	_AccelerationY += _deltaAccelerationY;
	_AccelerationY *= _SurfaceFriction;

	_AccelerationX = std::clamp(_AccelerationX, _MinAccelerationX, _MaxAccelerationX);
	_AccelerationY = std::clamp(_AccelerationY, _MinAccelerationY, _MaxAccelerationY);

	if ((_AccelerationX >= -0.01) && (_AccelerationX <= 0.01))
	{
		_AccelerationX = 0;
	}
	// else
	// {
	// 	std::cout << std::format("_AccelerationX: {}.\n", _AccelerationX);
	// }
	if ((_AccelerationY >= -0.01) && (_AccelerationY <= 0.01))
	{
		_AccelerationY = 0;
	}
	// else
	// {
	// 	std::cout << std::format("_AccelerationY: {}.\n", _AccelerationY);
	// }

	// std::cout << std::format("_AccelerationX: {}, _AccelerationY: {}.\n", _AccelerationX, _AccelerationY);
}

// void PhysicsEngine::applyGravity(float delta_t)
// {
// 	std::cout << "Gravity: " << !(_onPlatform || _onFrame || !_bIsEnabled) * _GRAVITY * double(delta_t) * _AirFrictionY
// 			  << '\n';
// 	_VelocityY += !(_onPlatform || _onFrame || !_bIsEnabled) * _GRAVITY * double(delta_t) * _AirFrictionY;
// }

void PhysicsEngine::move()
{
	// std::cout << "_forceX: " << _forceX << '\n';
	// std::cout << "_forceY: " << _forceY << '\n';
	// std::cout << "_AccelerationX: " << _AccelerationX << '\n';
	// std::cout << "_AccelerationY: " << _AccelerationY << '\n';
	// std::cout << "_VelocityX: " << _VelocityX << '\n';
	// std::cout << "_VelocityY: " << _VelocityY << '\n';
	// std::cout << "_deltaX: " << _deltaX << '\n';
	// std::cout << "_deltaY: " << _deltaY << '\n';
	// std::cout << "_X: " << _X << '\n';
	// std::cout << "_Y: " << _Y << '\n';

	//? Keep precise values
	_X += _deltaX;
	_Y += _deltaY;

	//? Retrieve whole values
	_hostBody->x = static_cast<int>(_X);
	_hostBody->y = static_cast<int>(_Y);
}

void PhysicsEngine::move(Point point)
{
	//? Moving object only if parsed any value

	if (point.x != 0)
	{
		//? Keep precise values
		_X += point.x;
		_deltaX = 0;
		//? Retrieve whole values
		_hostBody->x = static_cast<int>(_X);
	}

	if (point.y != 0)
	{
		//? Keep precise values
		_Y += point.y;
		_deltaY = 0;
		//? Retrieve whole values
		_hostBody->y = static_cast<int>(_Y);
	}
}

void PhysicsEngine::moveByCamera(Point point) { move(point); }

void PhysicsEngine::addForceX(float deltaForceX) { _forceX += deltaForceX; }

void PhysicsEngine::addForceY(float deltaForceY) { _forceY += deltaForceY; }

void PhysicsEngine::clearForceX() { _forceX = 0; }

void PhysicsEngine::clearForceY() { _forceY = 0; }

void PhysicsEngine::enableGravity(bool enable)
{
	// std::cout << "gravity enabled...\n";
	_bIsGravityEnabled = enable;
}

void PhysicsEngine::updateGravityOnly(float delta_t)
{
	_VelocityX *= _SurfaceFriction;
	_VelocityX = std::clamp(_VelocityX, _MinVelocityX, _MaxVelocityX);

	_VelocityY += !(_bOnPlatform || _bOnFrame || !_bIsGravityEnabled) * _GRAVITY * double(delta_t) * _AirFrictionY;
	_VelocityY += !_bIsGravityEnabled * std::abs(_VelocityY) * double(delta_t) * _AirFrictionY;
	_VelocityY *= _SurfaceFriction;
	_VelocityY = std::clamp(_VelocityY, _MinVelocityY, _MaxVelocityY);

	// std::cout << std::format("_VelocityX: {}, _VelocityY: {}.\n", _VelocityX, _VelocityY);
}

Rectangle* PhysicsEngine::getHost() { return _hostBody; }

double PhysicsEngine::getDeltaTheta() const { return _deltaTheta; }

double PhysicsEngine::getTheta() const { return *_hostTheta; }

double PhysicsEngine::getDeltaOmega() const { return _deltaOmega; }

double PhysicsEngine::getOmega() const { return _Omega; }

PhysicsEngine::Speed_2D PhysicsEngine::getSpeed() const { return { _VelocityX, _VelocityY }; }

LinearSpeed PhysicsEngine::getLinearSpeed() { return LinearSpeed(_VelocityX, _VelocityY); }

PhysicsEngine::Acceleration_2D PhysicsEngine::getAcceleration() const { return { _AccelerationX, _AccelerationY }; }

Deltas PhysicsEngine::getMoveDeltas() const { return Deltas(_deltaX, _deltaY); }

Deltas PhysicsEngine::getForces() const { return Deltas(_forceX, _forceY); }

bool PhysicsEngine::getIsJumping() const { return _bIsJumping; }

bool PhysicsEngine::getIsFalling() const { return _bIsFalling; }

bool PhysicsEngine::getOnPlatform() const { return _bOnPlatform; }

bool PhysicsEngine::getIsGravityEnabled() const { return _bIsGravityEnabled; }
