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
	: _Screen(parent_screen)
	, _SpawnPointHeight(Sizes::Player::DefaultSpawnPointHeight * _Screen->ScaleHeight)
	, _FlightDestination(_Screen->ApplicationHeight / 4)
	, _ImagePath(path)
{
	initialize();
	// std::cout << "_FlightDestination: " << _FlightDestination << '\n';
}

Player::~Player() {}

RectangleCore* Player::getPlayerBoundary() { return dynamic_cast<RectangleCore*>(_DoodieAnimation.get()); }

RectangleShape* Player::getPlayerShape() { return dynamic_cast<RectangleShape*>(_DoodieAnimation.get()); }

IRelocatableActor* Player::getPlayerLocator() { return dynamic_cast<IRelocatableActor*>(_DoodieLocator.get()); }

// PhysicsEngine* Player::getPlayerFullEngine() { return _DoodieMover.get(); }

IMechanics* Player::getPlayerEngine() { return dynamic_cast<IMechanics*>(_DoodieMover.get()); }

const Point* Player::getSpawnPoint() { return updateSpawnPoint(); }

void Player::initialize()
{
	_DoodieAnimation = std::make_unique<AnimatedImage>(_Screen,						   //
													   _ImagePath, 2,				   //
													   Sizes::Player::DefaultWidth,	   //
													   Sizes::Player::DefaultHeight	   //
	);

	_DoodieSpawnPoint = std::make_unique<Point>(0, _SpawnPointHeight);
	_CurrentLookSide = WalkingSide::LEFT;

	_DoodieLocator = std::make_unique<Locator>(_DoodieAnimation.get());

	_DoodieMover = std::make_unique<PhysicsEngine>(_DoodieAnimation.get());
	_DoodieMover->setSurfaceFriction(0.4);
	_DoodieMover->setAirFrictionY(0.015);

	_DoodieMover->setMass(_Mass);
	_DoodieMover->setIsJumping(false);

	// TODO: fix this
	//! Combination Locator and PE not working correctly
	//! Camera statter
	// _DoodieLocator->setPhysicsEngineConnection(_DoodieMover.get());

	_bIsShooting = false;
}

void Player::setInitialPlace(Point coordinate)
{
	_DoodieLocator->setBottomCLocation(coordinate.x, coordinate.y);
	// TODO: connection to physics engine works badly, but this call also not desire
	// TODO: need to set by locator and telling to PE at the same time
	_DoodieMover->setPreciseCoordinate(_DoodieLocator->getX(), _DoodieLocator->getY());
}

void Player::enableGravity(bool enable) { _DoodieMover->enableGravity(enable); }

void Player::clear()
{
	_DoodieSpawnPoint.reset();
	_DoodieMover.reset();
	_DoodieLocator.reset();
	_DoodieAnimation.reset();

	_bIsShooting = false;
}

void Player::tick(float delta_t)
{
	//? Synchonizing with wormhole changes
	// TODO: can we do this in the wormhole class?
	// _DoodieMover->setPreciseCoordinate(_DoodieLocator->getX(), _DoodieLocator->getY());

	if (_DoodieMover->getOnPlatform())
	{
		_DoodieMover->setAirFrictionY(0.015);
	}

	if (_DoodieMover->getIsJumping() && !_DoodieMover->getIsFalling())
	{
		if (!_bForceAdded)
		{
			_DoodieMover->clearForceX();
			_DoodieMover->clearForceY();

			auto Deltas = _DoodieMover->getLinearSpeed();

			_DoodieMover->addForceX(Deltas._Vx * _SideTilt);

			_JumpImpulse -= std::abs(Deltas._Vx) * 50;
			_DoodieMover->addForceY(_JumpImpulse);

			_DoodieMover->accelerateX(Deltas._Vx * _SideTilt);
			_DoodieMover->accelerateY(_JumpImpulse / 100);

			_bForceAdded = true;
		}

		_DoodieMover->updateImpulse(delta_t);
		_DoodieMover->updatePositions(delta_t);
		_DoodieMover->move();

		auto Deltas = _DoodieMover->getMoveDeltas();
		_CurrentFlightDistance += std::sqrt(std::pow(Deltas.x, 2) + std::pow(Deltas.y, 2));

		if (_CurrentFlightDistance >= _FlightDestination)
		{
			_DoodieMover->setIsJumping(false);
			_DoodieMover->clearVelocity();
			_DoodieMover->clearAccelerationX();
			_DoodieMover->clearAccelerationY();
			_DoodieMover->clearForceX();
			_DoodieMover->clearForceY();

			_DoodieMover->setIsFalling(true);
		}
	}
	else if (_DoodieMover->getIsFalling())
	{
		_DoodieMover->updateGravityOnly(delta_t);
		_DoodieMover->updateVelocities(delta_t);
		_DoodieMover->updatePositions(delta_t);
		_DoodieMover->move();

		if (_DoodieMover->getOnPlatform())
		{
			_DoodieMover->setIsFalling(false);
			_DoodieMover->clearAll();
		}

		_DoodieMover->setIsJumping(false);
	}
	else
	{
		_DoodieMover->updateVelocities(delta_t);
		_DoodieMover->updatePositions(delta_t);
		_DoodieMover->updateAccelerations(delta_t);
		_DoodieMover->move();

		_bForceAdded = false;

		_CurrentFlightDistance = 0;
	}
}

const Point* Player::updateSpawnPoint()
{
	_DoodieSpawnPoint->y = _DoodieAnimation->bottom() - _SpawnPointHeight;
	if (_CurrentLookSide == WalkingSide::LEFT)
	{
		_DoodieSpawnPoint->x = _DoodieAnimation->left() + 0;
	}
	else if (_CurrentLookSide == WalkingSide::RIGHT)
	{
		_DoodieSpawnPoint->x = _DoodieAnimation->left() + _DoodieAnimation->width();
	}

	return _DoodieSpawnPoint.get();
}

void Player::onWalking(WalkingSide side)
{
	//? Change frame for animation if not shooting
	//? Move toward desired side
	if (side == WalkingSide::LEFT)
	{
		_DoodieMover->accelerateX(-0.005f);
		_DoodieMover->speedUpX(-0.02f);
		_DoodieMover->addForceX(-1.5f);
	}
	else if (side == WalkingSide::RIGHT)
	{
		_DoodieMover->accelerateX(0.005f);
		_DoodieMover->speedUpX(0.02f);
		_DoodieMover->addForceX(1.5f);
	}

	//? We can change shooting side only we not shooting right now
	if (!_bIsShooting)
	{
		changeShootingSide(side);
	}
}

void Player::onUpArrowClicked()
{
	_DoodieMover->accelerateY(-0.005f);
	_DoodieMover->speedUpY(-0.02f);
	_DoodieMover->addForceY(-0.00005f);
}

void Player::onStopWalking()
{
	//? Stop walking
	_DoodieMover->clearAccelerationX();
	_DoodieMover->clearForceX();
}

void Player::onVerticalArrowsReleased()
{
	//? Stop jet
	_DoodieMover->clearAccelerationY();
	_DoodieMover->clearForceY();
}

void Player::render() { _DoodieAnimation->render(); }

void Player::changeShootingSide(WalkingSide side)
{
	_CurrentLookSide = side;

	if (_CurrentLookSide == WalkingSide::LEFT)
	{
		_DoodieAnimation->setCharacterFrame(0);
	}
	else if (_CurrentLookSide == WalkingSide::RIGHT)
	{
		_DoodieAnimation->setCharacterFrame(1);
	}
}

int Player::getBoundaryTop() { return _DoodieAnimation->top(); }

int Player::getCenterY() { return _DoodieLocator->getCenterY(); }

void Player::setShootingSide(const Point& aim)
{
	_bIsShooting = true;
	//? Check where is player center.x and where is aim.x and rotate character
	WalkingSide ShootingSide(WalkingSide::NONE);
	if (_DoodieLocator->getCenterX() <= aim.x)
	{
		ShootingSide = WalkingSide::RIGHT;
	}
	else
	{
		ShootingSide = WalkingSide::LEFT;
	}

	changeShootingSide(ShootingSide);
}

void Player::resetShooting() { _bIsShooting = false; }

void Player::setIsOnTopPlatfrom(bool new_state) { _DoodieMover->setOnTopOfAnyPlatform(new_state); }
