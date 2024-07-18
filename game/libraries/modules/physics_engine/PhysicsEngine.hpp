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
	void updateTheta(float deltaT);
	void updateOmega(float deltaT);
	void rotate();

	//* Manipulators: Linear movement

	//? Call after button pressed or any other condition
	void accelerateX(float deltaAccelerationX);
	virtual void accelerateY(float deltaAccelerationY) override;
	void clearAccelerationX();
	virtual void clearAccelerationY() override;

	void clearAll();

	void speedUpX(float deltaVelocityX);
	virtual void speedUpY(float deltaVelocityY) override;

	Deltas constantSpeedUp(float delta_t, float delta_x, float delta_y);
	virtual void constantSpeedUpY(float delta_t, float delta_y) override;

	//? Call every frame to update linear parameters and move
	void updateImpulse(float deltaT);
	void updatePositions(float deltaT);
	void updateVelocities(float deltaT);
	void updateAmmoAccelerations(float deltaT);
	void updateAccelerations(float deltaT);
	virtual void move() override;
	virtual void move(Point point) override;
	virtual void moveByCamera(Point point) override;

	//* Manipulators: Forcing

	void addForceX(float deltaForceX);
	virtual void addForceY(float deltaForceY) override;

	void clearForceX();
	virtual void clearForceY() override;

	void enableGravity(bool enable);
	void updateGravityOnly(float delta_t);

	//* Getters

	//? Host API
	Rectangle* getHost();

	//? Angle movement
	double getDeltaTheta() const;
	double getTheta() const;
	double getDeltaOmega() const;
	double getOmega() const;

	//? Linear movement
	Speed_2D getSpeed() const;	  //!
	virtual LinearSpeed getLinearSpeed() override;
	Acceleration_2D getAcceleration() const;
	Deltas getMoveDeltas() const;
	Deltas getForces() const;

	//? States
	virtual bool getIsJumping() const override;
	virtual bool getIsFalling() const override;
	virtual bool getOnPlatform() const override;
	bool getIsGravityEnabled() const;

	//* Setters

	//! Need to link Locator with PhysicsEngine cause after relocate engine don't know about it
	//? Save only when it's different, so precise data won't be loosed
	virtual void setPreciseCoordinate(int x, int y) override;
	virtual void setCoordinate(int x, int y) override;

	//? Angle movement
	bool speedUpOmega(double wSource, double wDestination, float timeDuration, float deltaTime);
	void setOmega(double omega);
	void clearOmega();
	void clearDeltaOmega();
	void setDeltaTheta(double deltaTh);
	void clearDeltaTheta();

	//? Linear movement
	// bool speedUpVelocity(double wSource, double wDestination, float timeDuration, float deltaTime);
	void setVelocity(double velocityX, double velocityY);
	virtual void setConstantVelocity(double velocity_x, double velocity_y) override;
	void clearVelocity();
	// void clearDeltaVelocity();
	// void setDeltaAcceleration(double deltaAccelerationX, double deltaAccelerationY);
	// void clearDeltaAcceleration();
	// void clearDeltaAccelerationX();
	// void clearDeltaAccelerationY();

	//? Update body position
	void resetBody(Rectangle* body);

	//? Mechanics
	virtual void setOnTopOfFrame(bool NewValue) override;
	virtual void setOnTopOfAnyPlatform(bool NewValue) override;
	virtual void setIsJumping(bool new_state) override;
	virtual void setIsFalling(bool new_state) override;

	//* Setters: parameters
	void setMass(float NewMass);
	void setSurfaceFriction(double new_surface_friction);
	virtual void setAirFrictionY(double new_air_friction) override;

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
	void clearTimer();

	//@ Members

	//* Hosts
	Rectangle* _hostBody{ nullptr };
	double* _hostTheta{ nullptr };	  //? angle

	//* Deltas

	//? Angle movement
	double _deltaTheta{};	 //? angle
	double _deltaOmega{};	 //? velocity
	double _deltaAlpha{};	 //? acceleration

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
	bool _bOnPlatform{ true };
	bool _bOnFrame{ false };
	bool _bIsGravityEnabled{ false };
	bool _bIsJumping{ false };
	bool _bIsFalling{ false };

	//* Absolete

	//? Angle movement
	// double _Theta{}; //? angle
	double _Omega{};	//? velocity
	double _Alpha{};	//? acceleration

	//? Linear movement
	double _X{};
	double _Y{};
	double _VelocityX{};
	double _MaxVelocityX{ 0.3 };
	double _MinVelocityX{ -0.3 };
	double _VelocityY{};
	double _MaxVelocityY{ 0.3 };
	double _MinVelocityY{ -0.3 };
	double _AccelerationX{};
	double _MaxAccelerationX{ 5.5 };
	double _MinAccelerationX{ -5.5 };
	double _AccelerationY{};
	double _MaxAccelerationY{ 5.5 };
	double _MinAccelerationY{ -5.5 };

	double _ConstVelocityX{};
	double _ConstVelocityY{};

	//? Manipulators
	double _SurfaceFriction{ 0.002 };
	double _AccelerationFriction{ 0.003 };
	double _ImpulseX{};
	double _ImpulseY{};
	double _AirFrictionY{ 0.002 };
	double _ImpulseLoss{ 0.999 };

	double _Mass{ 0.25 };

	//* Utitlities
	float __SpentTime{};
};
