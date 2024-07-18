#pragma once

//* Game
//? Interfaces
//? Inheritance
#include "graphics/image/Image.hpp"

//* C++ std
#include <vector>

//* Forward declarations

class AnimatedImage : public Image
{
public:
	//? Wrap spriteS and scale to desired size against to application scale
	AnimatedImage(const ScreenItem* parent_screen,								 //
				  const char* path,												 //
				  const unsigned int& sprites_quantity,							 //
				  const int& destination_width, const int& destination_height	 //
	);
	virtual ~AnimatedImage();

	//@ API

	//* Manipulators

	void setCharacterFrame(size_t index);

private:
	//@ Methods

	//* Let's prevent copying of the module
	AnimatedImage(const AnimatedImage&) = delete;
	AnimatedImage& operator=(const AnimatedImage&) = delete;

	//@ Members

	//* Frames
	std::vector<Sprite*> _Sprites;

	//* Utilities
	const unsigned int _FramesQty{};
};
