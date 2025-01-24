//* Source header
#include "projectiles/ammo/Ammo.hpp"

//* Custom
//? Modules
#include "modules/locator/Locator.hpp"
#include "modules/physics_engine/PhysicsEngine.hpp"
//? Wrappers
#include "graphics/image/Image.hpp"
//? Worlds
//? Utilities
#include "screen/ScreenItem.hpp"
#include "utilities/Sizes.hpp"

//* C++ std
#include <cmath>
#include <format>
#include <iostream>

Ammo::Ammo(const ScreenItem* parent_screen, const Point& begin, const Point& end, const char* path)
	: _screen(parent_screen), _imagePath(path)
{
	//? Construct default image
	_image = std::make_unique<Image>(_screen, _imagePath,			   //
									 Sizes::Projectiles::AmmoWidth,	   //
									 Sizes::Projectiles::AmmoWidth	   //
	);

	//? Construct locator
	_locator = std::make_unique<Locator>(_image.get());	   //? This is moving with camera
	//? Place projectile where it starts it's way
	_locator->SetCenterLocation(begin);

	//? Where to aim
	_destination = std::make_unique<Point>(end);	//? This is moving with camera

	//? Construct physics engine
	_mover = std::make_unique<PhysicsEngine>(_image.get());

	//? Calculate cathetus velocities
	double DeltaY = static_cast<double>((end.y - begin.y));
	int SignY = (DeltaY > 0) ? 1 : -1;
	double DeltaX = static_cast<double>((end.x - begin.x));
	int SignX = (DeltaX > 0) ? 1 : -1;
	double Alpha = std::atan(std::abs(DeltaY) / std::abs(DeltaX));
	double CathetusVelocityX = _constantVelocityX * std::cos(Alpha);
	double CathetusVelocityY = _constantVelocityY * std::sin(Alpha);

	//? Calculated according aim point angles:
	//? Const part
	_mover->SetConstantVelocity(CathetusVelocityX, CathetusVelocityY);
	//? Dynamic part
	_mover->SetMass(_mass);
	_mover->AddForceX(5.2f * SignX * float(CathetusVelocityX));
	_mover->AddForceY(5.2f * SignY * float(CathetusVelocityY));
	_mover->AccelerateX(1.5f * SignX * float(CathetusVelocityX));
	_mover->AccelerateY(1.5f * SignY * float(CathetusVelocityY));
	_mover->SetAirFrictionY(0.008);

	_locator->SetPhysicsEngineConnection(_mover.get());
	// _Locator->setLoggingEnabled(true);
}

Ammo::~Ammo() {}

IRelocatableActor* Ammo::GetLocator() { return _locator.get(); }

RectangleCore* Ammo::GetBoundary() { return _image.get(); }

IMechanics* Ammo::GetEngine() { return _mover.get(); }

bool Ammo::IsOnPlace() const { return _bIsOnPlace; }

void Ammo::Tick(float delta_t)
{
	//? Firtsly projectile goes with constant speed
	if (															   //
		((std::abs(_destination->x - _locator->GetCenterX()) > 20)	   //
		 ||															   //
		 (std::abs(_destination->y - _locator->GetCenterY()) > 20))	   //
		&&															   //
		!(_mover->GetIsGravityEnabled())							   //
		)															   //
	{
		_mover->ConstantSpeedUp(								//
			delta_t,											//
			float(_destination->x - _locator->GetCenterX()),	//
			float(_destination->y - _locator->GetCenterY()));
		_mover->Move();
	}
	//? later enable gravity
	//* It will cause ammos going down screen and eventually they would be deleted
	else
	{
		// _bIsOnPlace = true;

		_mover->UpdateAmmoAccelerations(delta_t);
		_mover->UpdateImpulse(delta_t);
		_mover->UpdatePositions(delta_t);
		_mover->Move();

		_mover->EnableGravity(true);
	}
}

void Ammo::Relocate(int deltaX, int deltaY)
{
	_locator->Relocate(deltaX, deltaY);
	_destination->x += deltaX;
	_destination->y += deltaY;
}

void Ammo::Render() { _image->Render(); }
