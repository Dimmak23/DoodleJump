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
	ResetBody(rectangle->GetBody());
	_hostTheta = rectangle->GetAngle();

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

void PhysicsEngine::SetPreciseCoordinate(int x, int y)
{
	if (static_cast<int>(_x) != x)
	{
		_x = x;
	}
	if (static_cast<int>(_y) != y)
	{
		_y = y;
	}
}

void PhysicsEngine::SetCoordinate(int x, int y) { SetPreciseCoordinate(x, y); }

bool PhysicsEngine::SpeedUpOmega(double wSource, double wDestination, float timeDuration, float deltaTime)
{
	if (_spentTime < timeDuration)
	{
		_alpha = (wDestination - wSource) / double(timeDuration);
		_spentTime += deltaTime;
		return false;
	}
	else
	{
		_alpha = 0;
		_spentTime = 0;
		return true;
	}
}

void PhysicsEngine::SetDeltaTheta(double deltaTh) { _deltaTheta = deltaTh; }

void PhysicsEngine::ClearDeltaTheta() { _deltaTheta *= 0; }

void PhysicsEngine::SetVelocity(double velocity_x, double velocity_y)
{
	_velocityX = velocity_x;
	_velocityY = velocity_y;
}

void PhysicsEngine::SetConstantVelocity(double velocity_x, double velocity_y)
{
	_constVelocityX = velocity_x;
	_constVelocityY = velocity_y;
}

void PhysicsEngine::ClearVelocity()
{
	_velocityX *= 0;
	_velocityY *= 0;
}

void PhysicsEngine::ResetBody(Rectangle* body)
{
	// std::cout << "resetBody(Rectangle* body)...\n";
	_hostBody = body;
	_x = _hostBody->x;
	_y = _hostBody->y;
	// std::cout << "_X: " << _X << '\n';
	// std::cout << "_Y: " << _Y << '\n';
}

void PhysicsEngine::SetMass(float NewMass) { _mass = NewMass; }

void PhysicsEngine::SetSurfaceFriction(double new_surface_friction) { _surfaceFriction = new_surface_friction; }

void PhysicsEngine::SetAirFrictionY(double new_air_friction) { _airFrictionY = new_air_friction; }

void PhysicsEngine::SetOnTopOfFrame(bool NewValue) { _bOnFrame = NewValue; }

void PhysicsEngine::SetOnTopOfAnyPlatform(bool NewValue)
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

void PhysicsEngine::SetIsJumping(bool new_state)
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

void PhysicsEngine::SetIsFalling(bool new_state) { _bIsFalling = new_state; }

void PhysicsEngine::SetOmega(double omega) { _omega = omega; }

void PhysicsEngine::ClearOmega() { _omega *= 0; }

void PhysicsEngine::ClearDeltaOmega() { _deltaOmega *= 0; }

void PhysicsEngine::ClearTimer() { _spentTime = 0; }

void PhysicsEngine::UpdateOmega(float deltaT)
{
	_deltaOmega = _alpha * double(deltaT);
	_omega += _deltaOmega;
}

void PhysicsEngine::UpdateTheta(float deltaT) { _deltaTheta = _omega * double(deltaT); }

void PhysicsEngine::Rotate()
{
	*_hostTheta += _deltaTheta;
	if (*_hostTheta >= 360) *_hostTheta -= 360;
}

void PhysicsEngine::AccelerateX(float deltaAccelerationX)
{
	// _AccelerationX += deltaAccelerationX;
	_deltaAccelerationX += deltaAccelerationX;
	// _deltaAccelerationX = std::clamp(_deltaAccelerationX, -10.f, 10.f);
	// _deltaAccelerationX = deltaAccelerationX;
}

void PhysicsEngine::AccelerateY(float deltaAccelerationY)
{
	//_AccelerationY += deltaAccelerationY;
	_deltaAccelerationY += deltaAccelerationY;
	// _deltaAccelerationY = std::clamp(_deltaAccelerationY, -10.f, 10.f);
	// _deltaAccelerationY = deltaAccelerationY;
}

void PhysicsEngine::ClearAccelerationX() { _deltaAccelerationX *= 0.98f; }

void PhysicsEngine::ClearAccelerationY() { _deltaAccelerationY *= 0.98f; }

void PhysicsEngine::ClearAll()
{
	_forceX = 0;
	_forceY = 0;
	_deltaVelocityX = 0;
	_deltaVelocityY = 0;
	_velocityX = 0;
	_velocityY = 0;
	_deltaAccelerationX = 0;
	_deltaAccelerationY = 0;
	_accelerationX = 0;
	_accelerationY = 0;
	_deltaX = 0;
	_deltaY = 0;
}

void PhysicsEngine::SpeedUpX(float deltaVelocityX) { _velocityX += double(deltaVelocityX); }

void PhysicsEngine::SpeedUpY(float deltaVelocityY) { _velocityY += double(deltaVelocityY); }

Deltas PhysicsEngine::ConstantSpeedUp(float delta_t, float delta_x, float delta_y)
{
	//? How much frames would need to move to the specific coordinate at X
	double _FramesX;
	_FramesX = (delta_x != 0) ? std::abs(double(delta_x) / (_constVelocityX * double(delta_t))) : 0;
	//? How much frames would need to move to the specific coordinate at Y
	double _FramesY;
	_FramesY = (delta_y != 0) ? std::abs(double(delta_y) / (_constVelocityY * double(delta_t))) : 0;

	_deltaX = (static_cast<int>(_FramesX) != 0) ? (delta_x / float(_FramesX)) : 0;
	_deltaY = (static_cast<int>(_FramesY) != 0) ? (delta_y / float(_FramesY)) : 0;

	return { _deltaX, _deltaY };
}

void PhysicsEngine::ConstantSpeedUpY(float delta_t, float delta_y)
{
	auto Unused = ConstantSpeedUp(delta_t, 0.f, delta_y);
	Unused;	   //!
}

void PhysicsEngine::UpdateImpulse(float deltaT)
{
	_forceX *= float(_impulseLoss);
	_velocityX = _forceX * deltaT / _mass;
	_velocityX += _accelerationX * double(deltaT);
	// _VelocityX *= _FrictionX;
	// _VelocityX *= double(1 - deltaT);

	_forceY *= float(_impulseLoss);
	_velocityY = _forceY * deltaT / _mass;
	_velocityY += _accelerationY * double(deltaT);
	_velocityY += _bIsGravityEnabled * (std::abs(_velocityY) + _GRAVITY) * double(deltaT) * _airFrictionY;
	// std::cout << std::format("gravity: {}.\n",
	// 						 _bIsEnabled * (std::abs(_VelocityY) + _GRAVITY) * double(deltaT) * _AirFrictionY);
	// std::cout << std::format("_VelocityX: {}, _VelocityY: {}.\n", _VelocityX, _VelocityY);
	// _VelocityY *= _FrictionY;
	// _VelocityY *= double(1 - deltaT);
}

void PhysicsEngine::UpdatePositions(float deltaT)
{
	_deltaX = float(_velocityX * double(deltaT));
	_deltaY = float(_velocityY * double(deltaT));
	// std::cout << std::format("_deltaX: {}, _deltaY: {}.\n", _deltaX, _deltaY);
}

void PhysicsEngine::UpdateVelocities(float deltaT)
{
	// _deltaVelocityX = _AccelerationX * double(deltaT);
	// _VelocityX += _deltaVelocityX;
	_velocityX += _accelerationX * double(deltaT);

	_velocityX += _forceX * deltaT / _mass;

	_velocityX *= _surfaceFriction;

	_velocityX = std::clamp(_velocityX, _minVelocityX, _maxVelocityX);

	// _deltaVelocityY = _AccelerationY * double(deltaT);
	// _VelocityY += _deltaVelocityY;
	_velocityY += _accelerationY * double(deltaT);

	// std::cout << "deltaVelY: "
	// 		  << !(_onPlatform || _onFrame || !_bIsEnabled) * _VelocityY * double(deltaT) * _AirFrictionY << '\n';
	// TODO: maybe increase gravity when camera is moving
	_velocityY += !(_bOnPlatform || _bOnFrame || !_bIsGravityEnabled) * _GRAVITY * double(deltaT) * _airFrictionY;
	_velocityY += !_bIsGravityEnabled * std::abs(_velocityY) * double(deltaT) * _airFrictionY;
	// std::cout << "grVel: " << !_bIsGravityEnabled * std::abs(_VelocityY) * double(deltaT) * _AirFrictionY << '\n';
	// std::cout << "gravity: "
	// 		  << !(_bOnPlatform || _bOnFrame || !_bIsGravityEnabled) * _GRAVITY * double(deltaT) * _AirFrictionY
	// 		  << '\n';

	_velocityY += _forceY * deltaT / _mass;

	_velocityY *= _surfaceFriction;

	_velocityY = std::clamp(_velocityY, _minVelocityY, _maxVelocityY);

	// std::cout << std::format("_VelocityX: {}, _VelocityY: {}.\n", _VelocityX, _VelocityY);
}

void PhysicsEngine::UpdateAmmoAccelerations(float deltaT)
{
	// TODO: friction*delta, and added to acceleration

	//  _deltaAccelerationX -= _FrictionX * _forceX / _Mass;
	auto SurfaceDecreaser = _surfaceFriction * deltaT * 100;
	auto AccDecreaser = _accelerationFriction * deltaT * 100;

	//! test this again
	// _AccelerationX += _deltaAccelerationX;
	// _AccelerationX *= _FrictionX;
	_deltaAccelerationX *= float(SurfaceDecreaser);
	_accelerationX += _deltaAccelerationX;
	_accelerationX *= AccDecreaser;

	// _deltaAccelerationY -= _FrictionY * _forceY / _Mass;

	//! test this again
	// _AccelerationY += _deltaAccelerationY;
	// _AccelerationY *= _FrictionY;
	_deltaAccelerationY *= float(SurfaceDecreaser);
	_accelerationY += double(_deltaAccelerationY);
	_accelerationY *= AccDecreaser;

	_accelerationX = std::clamp(_accelerationX, _minAccelerationX, _maxAccelerationX);
	_accelerationY = std::clamp(_accelerationY, _minAccelerationY, _maxAccelerationY);

	if ((_accelerationX >= -0.01) && (_accelerationX <= 0.01))
	{
		_accelerationX = 0;
	}
	else
	{
		// std::cout << std::format("_AccelerationX: {}.\n", _AccelerationX);
	}
	if ((_accelerationY >= -0.01) && (_accelerationY <= 0.01))
	{
		_accelerationY = 0;
	}
	else
	{
		// std::cout << std::format("_AccelerationY: {}.\n", _AccelerationY);
	}
}

void PhysicsEngine::UpdateAccelerations(float deltaT)
{
	// TODO: maybe apply force: dA = dF/M

	_accelerationX += _deltaAccelerationX;
	_accelerationX *= _surfaceFriction;

	_accelerationY += _deltaAccelerationY;
	_accelerationY *= _surfaceFriction;

	_accelerationX = std::clamp(_accelerationX, _minAccelerationX, _maxAccelerationX);
	_accelerationY = std::clamp(_accelerationY, _minAccelerationY, _maxAccelerationY);

	if ((_accelerationX >= -0.01) && (_accelerationX <= 0.01))
	{
		_accelerationX = 0;
	}
	// else
	// {
	// 	std::cout << std::format("_AccelerationX: {}.\n", _AccelerationX);
	// }
	if ((_accelerationY >= -0.01) && (_accelerationY <= 0.01))
	{
		_accelerationY = 0;
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

void PhysicsEngine::Move()
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
	_x += _deltaX;
	_y += _deltaY;

	//? Retrieve whole values
	_hostBody->x = static_cast<int>(_x);
	_hostBody->y = static_cast<int>(_y);
}

void PhysicsEngine::Move(Point point)
{
	//? Moving object only if parsed any value

	if (point.x != 0)
	{
		//? Keep precise values
		_x += point.x;
		_deltaX = 0;
		//? Retrieve whole values
		_hostBody->x = static_cast<int>(_x);
	}

	if (point.y != 0)
	{
		//? Keep precise values
		_y += point.y;
		_deltaY = 0;
		//? Retrieve whole values
		_hostBody->y = static_cast<int>(_y);
	}
}

void PhysicsEngine::MoveByCamera(Point point) { Move(point); }

void PhysicsEngine::AddForceX(float deltaForceX) { _forceX += deltaForceX; }

void PhysicsEngine::AddForceY(float deltaForceY) { _forceY += deltaForceY; }

void PhysicsEngine::ClearForceX() { _forceX = 0; }

void PhysicsEngine::ClearForceY() { _forceY = 0; }

void PhysicsEngine::EnableGravity(bool enable)
{
	// std::cout << "gravity enabled...\n";
	_bIsGravityEnabled = enable;
}

void PhysicsEngine::UpdateGravityOnly(float delta_t)
{
	_velocityX *= _surfaceFriction;
	_velocityX = std::clamp(_velocityX, _minVelocityX, _maxVelocityX);

	_velocityY += !(_bOnPlatform || _bOnFrame || !_bIsGravityEnabled) * _GRAVITY * double(delta_t) * _airFrictionY;
	_velocityY += !_bIsGravityEnabled * std::abs(_velocityY) * double(delta_t) * _airFrictionY;
	_velocityY *= _surfaceFriction;
	_velocityY = std::clamp(_velocityY, _minVelocityY, _maxVelocityY);

	// std::cout << std::format("_VelocityX: {}, _VelocityY: {}.\n", _VelocityX, _VelocityY);
}

Rectangle* PhysicsEngine::GetHost() { return _hostBody; }

double PhysicsEngine::GetDeltaTheta() const { return _deltaTheta; }

double PhysicsEngine::GetTheta() const { return *_hostTheta; }

double PhysicsEngine::GetDeltaOmega() const { return _deltaOmega; }

double PhysicsEngine::GetOmega() const { return _omega; }

PhysicsEngine::Speed_2D PhysicsEngine::GetSpeed() const { return { _velocityX, _velocityY }; }

LinearSpeed PhysicsEngine::GetLinearSpeed() { return LinearSpeed(_velocityX, _velocityY); }

PhysicsEngine::Acceleration_2D PhysicsEngine::GetAcceleration() const { return { _accelerationX, _accelerationY }; }

Deltas PhysicsEngine::GetMoveDeltas() const { return Deltas(_deltaX, _deltaY); }

Deltas PhysicsEngine::GetForces() const { return Deltas(_forceX, _forceY); }

bool PhysicsEngine::GetIsJumping() const { return _bIsJumping; }

bool PhysicsEngine::GetIsFalling() const { return _bIsFalling; }

bool PhysicsEngine::GetOnPlatform() const { return _bOnPlatform; }

bool PhysicsEngine::GetIsGravityEnabled() const { return _bIsGravityEnabled; }
