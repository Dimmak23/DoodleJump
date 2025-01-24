//* Source header
#include "abilities/jet_pack/JetPack.hpp"

//* Game
#include "modules/physics_engine/I_Mechanics.hpp"

//* C++ std
#include <format>
#include <iostream>

// TODO: engine should be assigned here and not be reset when level reloaded
JetPack::JetPack() {}

JetPack::~JetPack() {}

void JetPack::Init()
{
	_bIsEnabled = false;
	_jumpTimer = _jumpDuration;
	_bIsLoaded = true;
	_loadTimer = _loadDuration;
	_bJumping = false;
}

void JetPack::SetPhysicsEngineConnection(IMechanics* parent_engine) { _hostEngine = parent_engine; }

void JetPack::Tick(float delta_t)
{
	//? If enabled jet pack and it is loaded we can jump
	if ((_bIsEnabled || _bJumping) && (_jumpTimer > 0.f) && _bIsLoaded)
	{
		//? Jumping
		_hostEngine->ConstantSpeedUpY(delta_t, -100.f);
		// TODO: setted const speed in two independent places, somewhow work correctly
		_hostEngine->SetConstantVelocity(0.f, 0.525f);
		_hostEngine->Move();

		//? Tick timer
		_jumpTimer -= delta_t;
		// std::cout << std::format("Jumping: {} seconds.\n", _JumpTimer / 1000.f);

		//? Now we should be independent on player which releases button
		_bJumping = true;
	}
	else if (_jumpTimer <= 0.f)
	{
		//? Clear dynamics
		_hostEngine->SetIsFalling(true);
		_hostEngine->ClearForceY();

		//? We can start over again
		_jumpTimer = _jumpDuration;

		//? But only when player do enabled jet pack
		_bJumping = false;

		//? Now we need to wait until jet pack will be loaded
		_bIsLoaded = false;
	}

	//? Wait until jet pack will be loaded
	if (!_bIsLoaded && (_loadTimer > 0.f))
	{
		_loadTimer -= delta_t;
		// std::cout << std::format("\tRecharging...: {} seconds.\n", _LoadTimer / 1000.f);
	}
	else
	{
		_bIsLoaded = true;
		_loadTimer = _loadDuration;
	}
}

void JetPack::Enable(bool new_state) { _bIsEnabled = new_state; }

void JetPack::Clear() { _hostEngine = nullptr; }
