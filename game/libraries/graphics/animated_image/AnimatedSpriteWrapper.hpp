#pragma once

//* Custom
//? Interfaces
//? Inheritance
#include "SpriteWrapper.hpp"

//* C++ std
#include <vector>

//* Forward declarations

class AnimatedSpriteWrapper : public SpriteWrapper
{
public:
	//? Wrap spriteS and scale to desired size against to application scale
	AnimatedSpriteWrapper(const ScreenItem* parent_screen,								 //
						  const char* path,												 //
						  const unsigned int& sprites_quantity,							 //
						  const int& destination_width, const int& destination_height	 //
	);
	virtual ~AnimatedSpriteWrapper();

	//@ API

	//* Manipulators

	void setCharacterFrame(size_t index);

private:
	//@ Methods

	//* Let's prevent copying of the module
	AnimatedSpriteWrapper(const AnimatedSpriteWrapper&) = delete;
	AnimatedSpriteWrapper& operator=(const AnimatedSpriteWrapper&) = delete;

	//@ Members

	//* Frames
	std::vector<Sprite*> _Sprites;

	//* Utilities
	const unsigned int _FramesQty{};
};
