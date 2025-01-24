#pragma once

//* Game
#include "modules/locator/I_RelocatableActor.hpp"

//* C++ std
#include <memory>

//* Forward declarations
class Image;
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
	IRelocatableActor* GetLocator();
	RectangleCore* GetBoundary();
	IMechanics* GetEngine();

	//* Setters
	bool IsOnPlace() const;

	//* Manipulators
	void Tick(float delta_t);
	virtual void Relocate(int deltaX, int deltaY) override;

	//* Graphics
	void Render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	Ammo(const Ammo&) = delete;
	Ammo& operator=(const Ammo&) = delete;

	//@ Members

	//* Modules
	std::unique_ptr<Image> _image{ nullptr };
	std::unique_ptr<Locator> _locator{ nullptr };
	std::unique_ptr<PhysicsEngine> _mover{ nullptr };

	//* Destination
	std::unique_ptr<Point> _destination{ nullptr };

	//* Screen data
	const ScreenItem* _screen{ nullptr };

	//* Image path
	const char* _imagePath{ nullptr };

	//* Default parameters
	const double _constantVelocityX{ 0.89 };
	const double _constantVelocityY{ 0.89 };
	const double _precision{ 0.01 };
	float _initialImpulse{ 1.f };	 // TODO: implement impulse
	float _mass{ 10.f };			 // TODO: implement mass and gravity

	//* States
	bool _bIsOnPlace{ false };
};
