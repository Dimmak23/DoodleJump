
//* Source header
#include "Stealth.hpp"

//* C++ std
#include <format>
#include <iostream>

//* Custom
//? Inheritance
//? Interfaces
#include "I_Graphicle.hpp"
//? Modules
//? Wrappers
//? Projectiles
//? Abilities
//? Utilities

Stealth::Stealth() {}

Stealth::~Stealth() {}

bool Stealth::IsEnabled() const { return _bIsEnabled; }

IGraphicle* Stealth::getPlatformImage() const { return _PlatformImage; }

void Stealth::resetSpriteConnection(IGraphicle* image)
{
	_PlatformImage = image;
	_bIsEnabled = true;
	_VisibilityTimer = _VisibilityDuration;
	_bUnloaded = false;
}

void Stealth::emptySpriteConnection()
{
	_PlatformImage = nullptr;
	_bIsEnabled = false;
	_bUnloaded = false;
}

void Stealth::tick(float delta_t)
{
	if (!_bIsEnabled) return;

	if (_bUnloaded) return;

	if (_VisibilityTimer > 0.f)
	{
		//? Tick timer
		_VisibilityTimer -= delta_t;
		// std::cout << "_VisibilityTimer: " << _VisibilityTimer << '\n';
	}
	else
	{
		if (_PlatformImage)
		{
			_PlatformImage->setIsHidden(true);
		}

		_bUnloaded = true;
	}
}
