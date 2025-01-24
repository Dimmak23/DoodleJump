//* Source header
#include "abilities/stealth/Stealth.hpp"

//* Game
#include "graphics/interface/IGraphicle.hpp"

//* C++ std
#include <format>
#include <iostream>

Stealth::Stealth() {}

Stealth::~Stealth() {}

bool Stealth::IsEnabled() const { return _bIsEnabled; }

IGraphicle* Stealth::GetPlatformImage() const { return _platformImage; }

void Stealth::ResetSpriteConnection(IGraphicle* image)
{
	_platformImage = image;
	_bIsEnabled = true;
	_visibilityTimer = _visibilityDuration;
	_bUnloaded = false;
}

void Stealth::EmptySpriteConnection()
{
	_platformImage = nullptr;
	_bIsEnabled = false;
	_bUnloaded = false;
}

void Stealth::Tick(float delta_t)
{
	if (!_bIsEnabled) return;

	if (_bUnloaded) return;

	if (_visibilityTimer > 0.f)
	{
		//? Tick timer
		_visibilityTimer -= delta_t;
	}
	else
	{
		if (_platformImage)
		{
			_platformImage->SetIsHidden(true);
		}

		_bUnloaded = true;
	}
}
