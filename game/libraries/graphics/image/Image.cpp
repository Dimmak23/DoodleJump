//* Source header
#include "graphics/image/Image.hpp"

//* SDL2: Connector
#include "SDL2_Connector/Loger.hpp"
#include "SDL2_Connector/Render.hpp"

//* Game
//? Interfaces
#include "screen/ScreenItem.hpp"

//* C++ std
#include <iostream>

Image::Image(const char* path) : RectangleShape()
{
	_Sprite = Render::createSprite(path);
	Render::getSpriteSize(_Sprite, _rectangle.w, _rectangle.h);

	// LogLine("--c--\tConstructed Image size of: ", sizeof(*this));
}

Image::Image(const char* path, const int& application_width) : RectangleShape()	   //? Sprite will spread along X axis
{
	_Sprite = Render::createSprite(path);

	int original_width, original_height;
	Render::getSpriteSize(_Sprite, original_width, original_height);

	float scale_x = float(application_width) / float(original_width);
	Render::setSpriteSize(_Sprite, scale_x * original_width, scale_x * original_height);

	Render::getSpriteSize(_Sprite, _rectangle.w, _rectangle.h);

	// LogLine("--c--\tConstructed Image size of: ", sizeof(*this));
}

//! What's up with this?
Image::Image(const ScreenItem* parent_screen,	 //
			 const int& destination_width,		 //
			 const int& destination_height		 //
			 )
	: RectangleShape(parent_screen)	   //? width and height sets as disered
{
	_rectangle.w = _InitialScaleWidth * destination_width;
	_rectangle.h = _InitialScaleHeight * destination_height;
}

Image::Image(const ScreenItem* parent_screen,	 //
			 const char* path,					 //
			 const int& destination_width,		 //
			 const int& destination_height		 //
			 )
	: RectangleShape(parent_screen)	   //? width and height sets as disered
{
	_Sprite = Render::createSprite(path);
	Render::setSpriteSize(_Sprite, _InitialScaleWidth * destination_width, _InitialScaleHeight * destination_height);
	Render::getSpriteSize(_Sprite, _rectangle.w, _rectangle.h);

	// LogLine("--c--\tConstructed Image size of: ", sizeof(*this));
}

Image::~Image()
{
	Render::destroySprite(&_Sprite);

	// LogLine("--d--\tImage deconstructed...");
}

void Image::rescaleSprite(float scale_width, float scale_height)
{
	Render::setSpriteSize(_Sprite, scale_width * _rectangle.w, scale_height * _rectangle.h);
	Render::getSpriteSize(_Sprite, _rectangle.w, _rectangle.h);
}

void Image::setIsHidden(bool new_state) { _bIsHidden = new_state; }

void Image::render()
{
	if (!_bIsHidden)
	{
		Render::drawSprite(_Sprite, _rectangle.x, _rectangle.y);
	}
}
void Image::moveSprite(int x, int y)
{
	_rectangle.x += x;
	_rectangle.y += y;
}

void Image::placeSprite(int x, int y)
{
	_rectangle.x = x;
	_rectangle.y = y;
}
