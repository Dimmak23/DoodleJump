//* Source header
#include "JetPack.hpp"

//* C++ std
#include <format>
#include <iostream>

//* Custom
//? Inheritance
//? Interfaces
#include "I_Mechanics.hpp"
//? Modules
//? Wrappers
//? Worlds
//? Utilities

// TODO: engine should be assigned here and not be reset when level reloaded
JetPack::JetPack() {}

JetPack::~JetPack() {}

void JetPack::initialize()
{
	_bIsEnabled = false;
	_JumpTimer = _JumpDuration;
	_bIsLoaded = true;
	_LoadTimer = _LoadDuration;
	_bJumping = false;
}

void JetPack::setPhysicsEngineConnection(IMechanics* parent_engine) { _hostEngine = parent_engine; }

void JetPack::tick(float delta_t)
{
	//? If enabled jet pack and it is loaded we can jump
	if ((_bIsEnabled || _bJumping) && (_JumpTimer > 0.f) && _bIsLoaded)
	{
		//? Jumping
		_hostEngine->constantSpeedUpY(delta_t, -100.f);
		// TODO: setted const speed in two independent places, somewhow work correctly
		_hostEngine->setConstantVelocity(0.f, 0.525f);
		_hostEngine->move();

		//? Tick timer
		_JumpTimer -= delta_t;
		// std::cout << std::format("Jumping: {} seconds.\n", _JumpTimer / 1000.f);

		//? Now we should be independent on player which releases button
		_bJumping = true;
	}
	else if (_JumpTimer <= 0.f)
	{
		//? Clear dynamics
		_hostEngine->setIsFalling(true);
		_hostEngine->clearForceY();

		//? We can start over again
		_JumpTimer = _JumpDuration;

		//? But only when player do enabled jet pack
		_bJumping = false;

		//? Now we need to wait until jet pack will be loaded
		_bIsLoaded = false;
	}

	//? Wait until jet pack will be loaded
	if (!_bIsLoaded && (_LoadTimer > 0.f))
	{
		_LoadTimer -= delta_t;
		// std::cout << std::format("\tRecharging...: {} seconds.\n", _LoadTimer / 1000.f);
	}
	else
	{
		_bIsLoaded = true;
		_LoadTimer = _LoadDuration;
	}
}

void JetPack::enable(bool new_state) { _bIsEnabled = new_state; }

void JetPack::clear() { _hostEngine = nullptr; }
