#pragma once

//* Game
#include "modules/physics_engine/I_Connectable.hpp"
#include "modules/physics_engine/I_Mechanics.hpp"

//* C++ std
#include <numbers>	  // TODO: get rid of this later

//* Forward declarations
struct Rectangle;
struct Point;
class RectangleShape;

struct Deltas
{
	float x{};
	float y{};
};

class PhysicsEngine : public IMechanics, public IConnectable
{
public:
	PhysicsEngine(RectangleShape* rectangle);	 //? Using RectangleShape cause rotation API variables leaves there
	~PhysicsEngine();

	//@ API

	struct Speed_2D
	{
		double _Vx, _Vy;
	};
	struct Acceleration_2D
	{
		double _Ax, _Ay;
	};

	//* Manipulators: Angle movement

	//? Call every frame to update angles and rotate
	void UpdateTheta(float deltaT);
	void UpdateOmega(float deltaT);
	void Rotate();

	//* Manipulators: Linear movement

	//? Call after button pressed or any other condition
	void AccelerateX(float deltaAccelerationX);
	virtual void AccelerateY(float deltaAccelerationY) override;
	void ClearAccelerationX();
	virtual void ClearAccelerationY() override;

	void ClearAll();

	void SpeedUpX(float deltaVelocityX);
	virtual void SpeedUpY(float deltaVelocityY) override;

	Deltas ConstantSpeedUp(float delta_t, float delta_x, float delta_y);
	virtual void ConstantSpeedUpY(float delta_t, float delta_y) override;

	//? Call every frame to update linear parameters and move
	void UpdateImpulse(float deltaT);
	void UpdatePositions(float deltaT);
	void UpdateVelocities(float deltaT);
	void UpdateAmmoAccelerations(float deltaT);
	void UpdateAccelerations(float deltaT);
	virtual void Move() override;
	virtual void Move(Point point) override;
	virtual void MoveByCamera(Point point) override;

	//* Manipulators: Forcing

	void AddForceX(float deltaForceX);
	virtual void AddForceY(float deltaForceY) override;

	void ClearForceX();
	virtual void ClearForceY() override;

	void EnableGravity(bool enable);
	void UpdateGravityOnly(float delta_t);

	//* Getters

	//? Host API
	Rectangle* GetHost();

	//? Angle movement
	double GetDeltaTheta() const;
	double GetTheta() const;
	double GetDeltaOmega() const;
	double GetOmega() const;

	//? Linear movement
	Speed_2D GetSpeed() const;
	virtual LinearSpeed GetLinearSpeed() override;
	Acceleration_2D GetAcceleration() const;
	Deltas GetMoveDeltas() const;
	Deltas GetForces() const;

	//? States
	virtual bool GetIsJumping() const override;
	virtual bool GetIsFalling() const override;
	virtual bool GetOnPlatform() const override;
	bool GetIsGravityEnabled() const;

	//* Setters

	//! Need to link Locator with PhysicsEngine cause after relocate engine don't know about it
	//? Save only when it's different, so precise data won't be loosed
	virtual void SetPreciseCoordinate(int x, int y) override;
	virtual void SetCoordinate(int x, int y) override;

	//? Angle movement
	bool SpeedUpOmega(double wSource, double wDestination, float timeDuration, float deltaTime);
	void SetOmega(double omega);
	void ClearOmega();
	void ClearDeltaOmega();
	void SetDeltaTheta(double deltaTh);
	void ClearDeltaTheta();

	//? Linear movement
	// bool speedUpVelocity(double wSource, double wDestination, float timeDuration, float deltaTime);
	void SetVelocity(double velocityX, double velocityY);
	virtual void SetConstantVelocity(double velocity_x, double velocity_y) override;
	void ClearVelocity();
	// void clearDeltaVelocity();
	// void setDeltaAcceleration(double deltaAccelerationX, double deltaAccelerationY);
	// void clearDeltaAcceleration();
	// void clearDeltaAccelerationX();
	// void clearDeltaAccelerationY();

	//? Update body position
	void ResetBody(Rectangle* body);

	//? Mechanics
	virtual void SetOnTopOfFrame(bool NewValue) override;
	virtual void SetOnTopOfAnyPlatform(bool NewValue) override;
	virtual void SetIsJumping(bool new_state) override;
	virtual void SetIsFalling(bool new_state) override;

	//* Setters: parameters
	void SetMass(float NewMass);
	void SetSurfaceFriction(double new_surface_friction);
	virtual void SetAirFrictionY(double new_air_friction) override;

	//@ Members

	//? Constants
	static constexpr double _GRAVITY{ 9.80665 };
	// static const double _PI{ 3.14159265359 };
	static constexpr double _PI{ std::numbers::pi };

private:
	//@ Methods

	//* Let's prevent copying of the module
	PhysicsEngine(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(const PhysicsEngine&) = delete;

	//* Utilities
	void ClearTimer();

	//@ Members

	//* Hosts
	Rectangle* _hostBody{ nullptr };
	double* _hostTheta{ nullptr };	  //? angle

	//* Deltas

	//? Angle movement
	double _deltaTheta{};	 //? angle
	double _deltaOmega{};	 //? velocity
	double _deltaAlpha{};	 //? acceleration

	//* Absolete

	//? Angle movement
	// double _Theta{}; //? angle
	double _omega{};	//? velocity
	double _alpha{};	//? acceleration

	//? Linear movement
	double _x{};
	double _y{};
	double _velocityX{};
	double _maxVelocityX{ 0.3 };
	double _minVelocityX{ -0.3 };
	double _velocityY{};
	double _maxVelocityY{ 0.3 };
	double _minVelocityY{ -0.3 };
	double _accelerationX{};
	double _maxAccelerationX{ 5.5 };
	double _minAccelerationX{ -5.5 };
	double _accelerationY{};
	double _maxAccelerationY{ 5.5 };
	double _minAccelerationY{ -5.5 };

	double _constVelocityX{};
	double _constVelocityY{};

	//? Manipulators
	double _surfaceFriction{ 0.002 };
	double _accelerationFriction{ 0.003 };
	double _impulseX{};
	double _impulseY{};
	double _airFrictionY{ 0.002 };
	double _impulseLoss{ 0.999 };

	double _mass{ 0.25 };

	//* Utitlities
	float _spentTime{};

	//? Linear movement
	float _deltaX{};
	float _deltaY{};
	float _deltaVelocityX{};
	float _deltaVelocityY{};
	float _deltaAccelerationX{};
	float _deltaAccelerationY{};

	//? Manipulators
	float _deltaImpulseX{ 0.f };
	float _deltaImpulseY{ 0.f };
	float _forceX{};
	float _forceY{};

	//* States

	bool _bOnPlatform{ true };
	bool _bOnFrame{ false };
	bool _bIsGravityEnabled{ false };
	bool _bIsJumping{ false };
	bool _bIsFalling{ false };
};
