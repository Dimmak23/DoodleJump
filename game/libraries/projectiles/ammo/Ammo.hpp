#pragma once

//* DL_Framework

//* Custom
//? Modules
//? Inheritance
//? Interfaces
#include "I_RelocatableActor.hpp"

//* C++ std
#include <memory>

//* Forward declarations
class SpriteWrapper;
class Locator;
class PhysicsEngine;
struct Point;
struct ScreenItem;
class RectangleCore;
class IMechanics;

class Ammo : public IRelocatableActor
{
public:
	//? Spawn Ammo into Dynamic World by giving Start and End points
	// TODO: class Player need to have getSpawnPoint() method
	Ammo(const ScreenItem* parent_screen, const Point& begin, const Point& end, const char* path);
	~Ammo();

	//@ API

	//* Getters
	IRelocatableActor* getLocator();
	RectangleCore* getBoundary();
	IMechanics* getEngine();

	//* Setters
	bool IsOnPlace() const;

	//* Manipulators
	void tick(float delta_t);
	virtual void relocate(int deltaX, int deltaY) override;

	//* Graphics
	void render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	Ammo(const Ammo&) = delete;
	Ammo& operator=(const Ammo&) = delete;

	//@ Members

	//* Modules
	std::unique_ptr<SpriteWrapper> _Image{ nullptr };
	std::unique_ptr<Locator> _Locator{ nullptr };
	std::unique_ptr<PhysicsEngine> _Mover{ nullptr };

	//* Screen data
	const ScreenItem* _Screen{ nullptr };

	//* Destination
	std::unique_ptr<Point> _Destination{ nullptr };

	//* States
	bool _bIsOnPlace{ false };

	//* Default parameters
	float _InitialImpulse{ 1.f };	 // TODO: implement impulse
	float _Mass{ 10.f };			 // TODO: implement mass and gravity
	const double _ConstantVelocityX{ 0.89 };
	const double _ConstantVelocityY{ 0.89 };
	const double _Precision{ 0.01 };

	//* Image path
	const char* _ImagePath{ nullptr };
};
