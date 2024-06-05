//* Source header
#include "Ammo.hpp"

//* C++ std
#include <cmath>
#include <format>
#include <iostream>

//* Custom
//? Inheritance
#include "ScreenItem.hpp"
//? Interfaces
#include "I_RelocatableActor.hpp"
//? Modules
#include "Locator.hpp"
#include "PhysicsEngine.hpp"
//? Wrappers
#include "SpriteWrapper.hpp"
//? Worlds
//? Utilities
// #include "Path.hpp"
#include "Sizes.hpp"

Ammo::Ammo(const ScreenItem* parent_screen, const Point& begin, const Point& end, const char* path)
	: _Screen(parent_screen), _ImagePath(path)
{
	//? Construct default image
	_Image = std::make_unique<SpriteWrapper>(_Screen, _ImagePath,			   //
											 Sizes::Projectiles::AmmoWidth,	   //
											 Sizes::Projectiles::AmmoWidth	   //
	);

	//? Construct locator
	_Locator = std::make_unique<Locator>(_Image.get());	   //? This is moving with camera
	//? Place projectile where it starts it's way
	_Locator->setCenterLocation(begin);

	//? Where to aim
	_Destination = std::make_unique<Point>(end);	//? This is moving with camera

	//? Construct physics engine
	_Mover = std::make_unique<PhysicsEngine>(_Image.get());

	//? Calculate cathetus velocities
	double DeltaY = static_cast<double>((end.y - begin.y));
	int SignY = (DeltaY > 0) ? 1 : -1;
	double DeltaX = static_cast<double>((end.x - begin.x));
	int SignX = (DeltaX > 0) ? 1 : -1;
	double Alpha = std::atan(std::abs(DeltaY) / std::abs(DeltaX));
	double CathetusVelocityX = _ConstantVelocityX * std::cos(Alpha);
	double CathetusVelocityY = _ConstantVelocityY * std::sin(Alpha);

	//? Calculated according aim point angles:
	//? Const part
	_Mover->setConstantVelocity(CathetusVelocityX, CathetusVelocityY);
	//? Dynamic part
	_Mover->setMass(_Mass);
	_Mover->addForceX(5.2 * SignX * CathetusVelocityX);
	_Mover->addForceY(5.2 * SignY * CathetusVelocityY);
	_Mover->accelerateX(1.5 * SignX * CathetusVelocityX);
	_Mover->accelerateY(1.5 * SignY * CathetusVelocityY);
	_Mover->setAirFrictionY(0.008);

	_Locator->setPhysicsEngineConnection(_Mover.get());
	// _Locator->setLoggingEnabled(true);
}

Ammo::~Ammo() {}

IRelocatableActor* Ammo::getLocator() { return _Locator.get(); }

RectangleCore* Ammo::getBoundary() { return _Image.get(); }

IMechanics* Ammo::getEngine() { return _Mover.get(); }

bool Ammo::IsOnPlace() const { return _bIsOnPlace; }

void Ammo::tick(float delta_t)
{
	//? Firtsly projectile goes with constant speed
	if (															   //
		((std::abs(_Destination->x - _Locator->getCenterX()) > 20)	   //
		 ||															   //
		 (std::abs(_Destination->y - _Locator->getCenterY()) > 20))	   //
		&&															   //
		!(_Mover->getIsGravityEnabled())							   //
		)															   //
	{
		_Mover->constantSpeedUp(						 //
			delta_t,									 //
			_Destination->x - _Locator->getCenterX(),	 //
			_Destination->y - _Locator->getCenterY());
		_Mover->move();
	}
	//? later enable gravity
	//* It will cause ammos going down screen and eventually they would be deleted
	else
	{
		// _bIsOnPlace = true;

		_Mover->updateAmmoAccelerations(delta_t);
		_Mover->updateImpulse(delta_t);
		_Mover->updatePositions(delta_t);
		_Mover->move();

		_Mover->enableGravity(true);
	}
}

void Ammo::relocate(int deltaX, int deltaY)
{
	_Locator->relocate(deltaX, deltaY);
	_Destination->x += deltaX;
	_Destination->y += deltaY;
}

void Ammo::render() { _Image->render(); }
