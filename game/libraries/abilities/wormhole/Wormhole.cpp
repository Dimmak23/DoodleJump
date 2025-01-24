//* Source header
#include "abilities/wormhole/Wormhole.hpp"

//* Custom
#include "modules/locator/Locator.hpp"
#include "modules/physics_engine/I_Mechanics.hpp"
#include "modules/physics_engine/PhysicsEngine.hpp"
#include "rectangle/RectangleShape.hpp"
#include "screen/ScreenItem.hpp"

//* C++ std
#include <cmath>
#include <format>
#include <iostream>

Wormhole::Wormhole(const ScreenItem* parent_screen, IMechanics* character_engine, RectangleShape* character_body,
				   RectangleCore* aim_body)
	: _characterEngine(character_engine)
	, _possesingBody(character_body)
	, _aim(aim_body)
	, _activationDistance(int(0.25f * parent_screen->_applicationHeight))
{
	_possesingMover = std::make_unique<PhysicsEngine>(_possesingBody);
	_sharedLocator = std::make_unique<Locator>(nullptr);
}

Wormhole::~Wormhole() {}

bool Wormhole::IsStucked() const { return _bIsStacked; }

void Wormhole::Tick(float delta_t)
{
	//? Finding trajectory coordinates
	_sharedLocator->ResetBody(_aim->GetBody());
	auto Destination = _sharedLocator->GetCenter();
	_sharedLocator->ResetBody(_possesingBody->GetBody());
	auto Origin = _sharedLocator->GetCenter();

	//? How far character in this frame
	double DeltaX = static_cast<double>((Destination.x - Origin.x));
	double DeltaY = static_cast<double>((Destination.y - Origin.y));
	double Distance = std::sqrt(std::pow(DeltaX, 2) + std::pow(DeltaY, 2));

	if (																				//
		((Distance <= _activationDistance)												//
		 && (_characterEngine->GetIsFalling() || _characterEngine->GetIsJumping()))		//
		|| ((Distance <= (_aim->Width() * 2)) && _characterEngine->GetOnPlatform()))	//
	{
		//? Depending on how far character speed will be different
		//? For far character CurrentSpeed would be lower
		double CurrentSpeed =
			(																						  //
				(double(_aim->Width()) /* / 2 */) / ((double(_aim->Width()) /* / 2 */) + Distance)	  //
				) *
			_maximumLinearSpeed;

		//? Calculate cathetus velocities for speed in this frame
		int SignY = (DeltaY > 0) ? 1 : -1;
		int SignX = (DeltaX > 0) ? 1 : -1;
		double Alpha = std::atan(std::abs(DeltaY) / std::abs(DeltaX));
		double CathetusVelocityX = CurrentSpeed * std::cos(Alpha);
		double CathetusVelocityY = CurrentSpeed * std::sin(Alpha);

		//? Now set this const speeds
		_possesingMover->SetConstantVelocity(CathetusVelocityX, CathetusVelocityY);

		//? Actually move
		auto DeltasOnFrame = _possesingMover->ConstantSpeedUp(	  //
			delta_t,											  //
			float(Destination.x - Origin.x),					  //
			float(Destination.y - Origin.y)						  //
		);
		// std::cout << std::format("Wormhole found player and generates deltas x: {}, y: {}.\n", DeltasOnFrame.x,
		// 						 DeltasOnFrame.y);

		//? Coordinates could be changed by another physics engine
		_possesingMover->SetPreciseCoordinate(_possesingBody->Left(), _possesingBody->Top());
		_possesingMover->Move();

		//? Synchonizing player with wormhole changes
		_characterEngine->SetCoordinate(_possesingBody->Left(), _possesingBody->Top());
	}

	if (Distance <= (float(_aim->Width()) / 2))
	{
		// std::cout << "Stucked in the wormhole\n";
		_bIsStacked = true;
	}
}
