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
	: _CharacterEngine(character_engine)
	, _PossesingBody(character_body)
	, _Aim(aim_body)
	, _ActivationDistance(0.25f * parent_screen->ApplicationHeight)
{
	_PossesingMover = std::make_unique<PhysicsEngine>(_PossesingBody);
	_SharedLocator = std::make_unique<Locator>(nullptr);
}

Wormhole::~Wormhole() {}

bool Wormhole::IsStucked() const { return _bIsStacked; }

void Wormhole::tick(float delta_t)
{
	//? Finding trajectory coordinates
	_SharedLocator->resetBody(_Aim->getBody());
	auto Destination = _SharedLocator->getCenter();
	_SharedLocator->resetBody(_PossesingBody->getBody());
	auto Origin = _SharedLocator->getCenter();

	//? How far character in this frame
	double DeltaX = static_cast<double>((Destination.x - Origin.x));
	double DeltaY = static_cast<double>((Destination.y - Origin.y));
	double Distance = std::sqrt(std::pow(DeltaX, 2) + std::pow(DeltaY, 2));

	if (																				//
		((Distance <= _ActivationDistance)												//
		 && (_CharacterEngine->getIsFalling() || _CharacterEngine->getIsJumping()))		//
		|| ((Distance <= (_Aim->width() * 2)) && _CharacterEngine->getOnPlatform()))	//
	{
		//? Depending on how far character speed will be different
		//? For far character CurrentSpeed would be lower
		double CurrentSpeed =
			(																						  //
				(double(_Aim->width()) /* / 2 */) / ((double(_Aim->width()) /* / 2 */) + Distance)	  //
				) *
			_MaximumLinearSpeed;

		//? Calculate cathetus velocities for speed in this frame
		int SignY = (DeltaY > 0) ? 1 : -1;
		int SignX = (DeltaX > 0) ? 1 : -1;
		double Alpha = std::atan(std::abs(DeltaY) / std::abs(DeltaX));
		double CathetusVelocityX = CurrentSpeed * std::cos(Alpha);
		double CathetusVelocityY = CurrentSpeed * std::sin(Alpha);

		//? Now set this const speeds
		_PossesingMover->setConstantVelocity(CathetusVelocityX, CathetusVelocityY);

		//? Actually move
		auto DeltasOnFrame = _PossesingMover->constantSpeedUp(	  //
			delta_t,											  //
			Destination.x - Origin.x,							  //
			Destination.y - Origin.y							  //
		);
		// std::cout << std::format("Wormhole found player and generates deltas x: {}, y: {}.\n", DeltasOnFrame.x,
		// 						 DeltasOnFrame.y);

		//? Coordinates could be changed by another physics engine
		_PossesingMover->setPreciseCoordinate(_PossesingBody->left(), _PossesingBody->top());
		_PossesingMover->move();

		//? Synchonizing player with wormhole changes
		_CharacterEngine->setCoordinate(_PossesingBody->left(), _PossesingBody->top());
	}

	if (Distance <= (float(_Aim->width()) / 2))
	{
		// std::cout << "Stucked in the wormhole\n";
		_bIsStacked = true;
	}
}
