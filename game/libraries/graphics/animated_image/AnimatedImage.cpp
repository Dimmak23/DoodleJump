//* Source header
#include "graphics/animated_image/AnimatedImage.hpp"

//* SDL2: Connector
#include "SDL2_connector/Render.hpp"

//* C++ std
#include <format>
#include <iostream>
#include <string>

AnimatedImage::AnimatedImage(const ScreenItem* parent_screen,								//
							 const char* path, const unsigned int& sprites_quantity,		//
							 const int& destination_width, const int& destination_height	//
							 )
	: Image(parent_screen, destination_width, destination_height), _framesQty(sprites_quantity)
{
	std::string path_to_sprite(path);
	std::string delimeter;
	std::string adder(".png");
	for (size_t index{}; index < sprites_quantity; index++)
	{
		delimeter = std::to_string(index);
		_sprites.push_back(Render::CreateSprite((path_to_sprite + delimeter + adder).c_str()));
		//? Set initial size
		Render::SetSpriteSize(_sprites.back(), _rectangle.w, _rectangle.h);
	}

	//? Set inital sprite
	_sprite = _sprites[0];

	// std::cout << std::format("Constructed AnimatedImage with size: {}.\n", sizeof(*this));
}

AnimatedImage::~AnimatedImage()
{
	// std::cout << "Destructor ~AnimatedImage()\n";
	//? Releasing resources
	for (auto& sprite : _sprites)
	{
		//? Deleting only non-active frames
		if (sprite != _sprite)
		{
			Render::DestroySprite(&sprite);
			// std::cout << "Deleted single frame from AnimatedImage...\n";
		}
	}
	// std::cout << "Deleted AnimatedImage...\n";
}

void AnimatedImage::SetCharacterFrame(size_t index)
{
	if (index < _framesQty)
	{
		_sprite = _sprites[index];	  //? This will be rendered later
	}
}
