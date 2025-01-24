//* Source header
#include "actor/player/Player.hpp"

//* Game
#include "graphics/animated_image/AnimatedImage.hpp"
#include "modules/locator/Locator.hpp"
#include "modules/physics_engine/PhysicsEngine.hpp"
#include "screen/ScreenItem.hpp"
#include "utilities/Sizes.hpp"

//* C++ std
#include <cmath>
#include <format>
#include <iostream>

Player::Player(const ScreenItem* parent_screen, const char* path)
	: _screen(parent_screen)
	, _spawnPointHeight((const unsigned int)(Sizes::Player::DefaultSpawnPointHeight * _screen->_scaleHeight))
	, _flightDestination(_screen->_applicationHeight / 4)
	, _imagePath(path)
{
	Initialize();
	// std::cout << "_FlightDestination: " << _FlightDestination << '\n';
}

Player::~Player() {}

RectangleCore* Player::GetPlayerBoundary() { return dynamic_cast<RectangleCore*>(_doodieAnimation.get()); }

RectangleShape* Player::GetPlayerShape() { return dynamic_cast<RectangleShape*>(_doodieAnimation.get()); }

IRelocatableActor* Player::GetPlayerLocator() { return dynamic_cast<IRelocatableActor*>(_doodieLocator.get()); }

// PhysicsEngine* Player::GetPlayerFullEngine() { return _DoodieMover.get(); }

IMechanics* Player::GetPlayerEngine() { return dynamic_cast<IMechanics*>(_doodieMover.get()); }

const Point* Player::GetSpawnPoint() { return UpdateSpawnPoint(); }

void Player::Initialize()
{
	_doodieAnimation = std::make_unique<AnimatedImage>(_screen,						   //
													   _imagePath, 2,				   //
													   Sizes::Player::DefaultWidth,	   //
													   Sizes::Player::DefaultHeight	   //
	);

	_doodieSpawnPoint = std::make_unique<Point>(0, _spawnPointHeight);
	_currentLookSide = WalkingSide::LEFT;

	_doodieLocator = std::make_unique<Locator>(_doodieAnimation.get());

	_doodieMover = std::make_unique<PhysicsEngine>(_doodieAnimation.get());
	_doodieMover->SetSurfaceFriction(0.4);
	_doodieMover->SetAirFrictionY(0.015);

	_doodieMover->SetMass(_mass);
	_doodieMover->SetIsJumping(false);

	// TODO: fix this
	//! Combination Locator and PE not working correctly
	//! Camera statter
	// _DoodieLocator->setPhysicsEngineConnection(_DoodieMover.get());

	_bIsShooting = false;
}

void Player::SetInitialPlace(Point coordinate)
{
	_doodieLocator->SetBottomCLocation(coordinate.x, coordinate.y);
	// TODO: connection to physics engine works badly, but this call also not desire
	// TODO: need to set by locator and telling to PE at the same time
	_doodieMover->SetPreciseCoordinate(_doodieLocator->GetX(), _doodieLocator->GetY());
}

void Player::EnableGravity(bool enable) { _doodieMover->EnableGravity(enable); }

void Player::Clear()
{
	_doodieSpawnPoint.reset();
	_doodieMover.reset();
	_doodieLocator.reset();
	_doodieAnimation.reset();

	_bIsShooting = false;
}

void Player::Tick(float delta_t)
{
	//? Synchonizing with wormhole changes
	// TODO: can we do this in the wormhole class?
	// _DoodieMover->setPreciseCoordinate(_DoodieLocator->getX(), _DoodieLocator->getY());

	if (_doodieMover->GetOnPlatform())
	{
		_doodieMover->SetAirFrictionY(0.015);
	}

	if (_doodieMover->GetIsJumping() && !_doodieMover->GetIsFalling())
	{
		if (!_bForceAdded)
		{
			_doodieMover->ClearForceX();
			_doodieMover->ClearForceY();

			auto Deltas = _doodieMover->GetLinearSpeed();

			_doodieMover->AddForceX(float(Deltas._Vx * _sideTilt));

			_jumpImpulse -= float(std::abs(Deltas._Vx) * 50);
			_doodieMover->AddForceY(_jumpImpulse);

			_doodieMover->AccelerateX(float(Deltas._Vx) * _sideTilt);
			_doodieMover->AccelerateY(_jumpImpulse / 100);

			_bForceAdded = true;
		}

		_doodieMover->UpdateImpulse(delta_t);
		_doodieMover->UpdatePositions(delta_t);
		_doodieMover->Move();

		auto Deltas = _doodieMover->GetMoveDeltas();
		_currentFlightDistance += (unsigned int)(std::sqrt(std::pow(Deltas.x, 2) + std::pow(Deltas.y, 2)));

		if (_currentFlightDistance >= _flightDestination)
		{
			_doodieMover->SetIsJumping(false);
			_doodieMover->ClearVelocity();
			_doodieMover->ClearAccelerationX();
			_doodieMover->ClearAccelerationY();
			_doodieMover->ClearForceX();
			_doodieMover->ClearForceY();

			_doodieMover->SetIsFalling(true);
		}
	}
	else if (_doodieMover->GetIsFalling())
	{
		_doodieMover->UpdateGravityOnly(delta_t);
		_doodieMover->UpdateVelocities(delta_t);
		_doodieMover->UpdatePositions(delta_t);
		_doodieMover->Move();

		if (_doodieMover->GetOnPlatform())
		{
			_doodieMover->SetIsFalling(false);
			_doodieMover->ClearAll();
		}

		_doodieMover->SetIsJumping(false);
	}
	else
	{
		_doodieMover->UpdateVelocities(delta_t);
		_doodieMover->UpdatePositions(delta_t);
		_doodieMover->UpdateAccelerations(delta_t);
		_doodieMover->Move();

		_bForceAdded = false;

		_currentFlightDistance = 0;
	}
}

const Point* Player::UpdateSpawnPoint()
{
	_doodieSpawnPoint->y = _doodieAnimation->Bottom() - _spawnPointHeight;
	if (_currentLookSide == WalkingSide::LEFT)
	{
		_doodieSpawnPoint->x = _doodieAnimation->Left() + 0;
	}
	else if (_currentLookSide == WalkingSide::RIGHT)
	{
		_doodieSpawnPoint->x = _doodieAnimation->Left() + _doodieAnimation->Width();
	}

	return _doodieSpawnPoint.get();
}

void Player::OnWalking(WalkingSide side)
{
	//? Change frame for animation if not shooting
	//? Move toward desired side
	if (side == WalkingSide::LEFT)
	{
		_doodieMover->AccelerateX(-0.005f);
		_doodieMover->SpeedUpX(-0.02f);
		_doodieMover->AddForceX(-1.5f);
	}
	else if (side == WalkingSide::RIGHT)
	{
		_doodieMover->AccelerateX(0.005f);
		_doodieMover->SpeedUpX(0.02f);
		_doodieMover->AddForceX(1.5f);
	}

	//? We can change shooting side only we not shooting right now
	if (!_bIsShooting)
	{
		ChangeShootingSide(side);
	}
}

void Player::OnUpArrowClicked()
{
	_doodieMover->AccelerateY(-0.005f);
	_doodieMover->SpeedUpY(-0.02f);
	_doodieMover->AddForceY(-0.00005f);
}

void Player::OnStopWalking()
{
	//? Stop walking
	_doodieMover->ClearAccelerationX();
	_doodieMover->ClearForceX();
}

void Player::OnVerticalArrowsReleased()
{
	//? Stop jet
	_doodieMover->ClearAccelerationY();
	_doodieMover->ClearForceY();
}

void Player::Render() { _doodieAnimation->Render(); }

void Player::ChangeShootingSide(WalkingSide side)
{
	_currentLookSide = side;

	if (_currentLookSide == WalkingSide::LEFT)
	{
		_doodieAnimation->SetCharacterFrame(0);
	}
	else if (_currentLookSide == WalkingSide::RIGHT)
	{
		_doodieAnimation->SetCharacterFrame(1);
	}
}

int Player::GetBoundaryTop() { return _doodieAnimation->Top(); }

int Player::GetCenterY() { return _doodieLocator->GetCenterY(); }

void Player::SetShootingSide(const Point& aim)
{
	_bIsShooting = true;
	//? Check where is player center.x and where is aim.x and rotate character
	WalkingSide ShootingSide(WalkingSide::NONE);
	if (_doodieLocator->GetCenterX() <= aim.x)
	{
		ShootingSide = WalkingSide::RIGHT;
	}
	else
	{
		ShootingSide = WalkingSide::LEFT;
	}

	ChangeShootingSide(ShootingSide);
}

void Player::ResetShooting() { _bIsShooting = false; }

void Player::SetIsOnTopPlatfrom(bool new_state) { _doodieMover->SetOnTopOfAnyPlatform(new_state); }
