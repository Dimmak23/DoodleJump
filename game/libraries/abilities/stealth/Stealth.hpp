#pragma once

//* Forward declarations
class IGraphicle;
using size_t = unsigned long long;

/*
@ Construct shallow pointer to Stealth object
@ on Platforms iterations:
@ if character is on the platform:
@ call resetSpriteConnection(new_platform) - it resets all fields
@ call setIsEnabled(true) - it will invoke tick of the module
@ post-condition: module will hide sprite after two seconds of player being on the platform
*/
class Stealth
{
public:
	Stealth();
	~Stealth();

	//@ API for dynamic world

	//* Getters
	bool IsEnabled() const;
	IGraphicle* GetPlatformImage() const;

	//* Setters
	void ResetSpriteConnection(IGraphicle* image);
	void EmptySpriteConnection();

	//* Manipulators
	//? Checks if 'this' IsEnabled, ticks and hide platform
	void Tick(float delta_t);

private:
	//@ Methods

	//* Let's prevent copying of the module
	Stealth(const Stealth&) = delete;
	Stealth& operator=(const Stealth&) = delete;

	//@ Members

	IGraphicle* _platformImage{ nullptr };

	//* Timers
	float _visibilityTimer{ 2000.f };
	const float _visibilityDuration{ 2000.f };	  // ms

	//* States
	bool _bIsEnabled{ false };
	bool _bUnloaded{ false };
};
