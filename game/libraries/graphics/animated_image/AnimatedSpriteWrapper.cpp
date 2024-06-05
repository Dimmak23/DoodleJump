//* Source header
#include "AnimatedSpriteWrapper.hpp"

//* DL_Framework
#include "Framework.h"

//* C++ std
#include <format>
#include <iostream>
#include <string>

//* Custom

AnimatedSpriteWrapper::AnimatedSpriteWrapper(const ScreenItem* parent_screen,								//
											 const char* path, const unsigned int& sprites_quantity,		//
											 const int& destination_width, const int& destination_height	//
											 )
	: SpriteWrapper(parent_screen, destination_width, destination_height), _FramesQty(sprites_quantity)
{
	std::string path_to_sprite(path);
	std::string delimeter;
	std::string adder(".png");
	for (size_t index{}; index < sprites_quantity; index++)
	{
		delimeter = std::to_string(index);
		_Sprites.push_back(createSprite((path_to_sprite + delimeter + adder).c_str()));
		//? Set initial size
		setSpriteSize(_Sprites.back(), _rectangle.w, _rectangle.h);
	}

	//? Set inital sprite
	_sprite = _Sprites[0];

	// std::cout << std::format("Constructed AnimatedSpriteWrapper with size: {}.\n", sizeof(*this));
}

AnimatedSpriteWrapper::~AnimatedSpriteWrapper()
{
	// std::cout << "Destructor ~AnimatedSpriteWrapper()\n";
	//? Releasing resources
	for (auto& Sprite : _Sprites)
	{
		//? Deleting only non-active frames
		if (Sprite != _sprite)
		{
			destroySprite(Sprite);
			// std::cout << "Deleted single frame from AnimatedSpriteWrapper...\n";
		}
	}
	// std::cout << "Deleted AnimatedSpriteWrapper...\n";
}

void AnimatedSpriteWrapper::setCharacterFrame(size_t index)
{
	if (index < _FramesQty)
	{
		_sprite = _Sprites[index];	  //? This will be rendered later
	}
}
