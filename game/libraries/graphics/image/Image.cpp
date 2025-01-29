//* Source header
#include "graphics/image/Image.hpp"

//* SDL2: Connector
#include "SDL2_connector/Loger.hpp"
#include "SDL2_connector/Render.hpp"

//* Game
//? Interfaces
#include "screen/ScreenItem.hpp"

//* C++ std
#include <iostream>

Image::Image(const char* path) : RectangleShape()
{
	_sprite = Render::CreateSprite(path);
	Render::GetSpriteSize(_sprite, _rectangle.w, _rectangle.h);

	// LogLine("--c--\tConstructed Image size of: ", sizeof(*this));
}

Image::Image(const char* path, const int& application_width) : RectangleShape()	   //? Sprite will spread along X axis
{
	_sprite = Render::CreateSprite(path);

	int original_width, original_height;
	Render::GetSpriteSize(_sprite, original_width, original_height);

	float scale_x = float(application_width) / float(original_width);
	Render::SetSpriteSize(_sprite, int(scale_x * original_width), int(scale_x * original_height));

	Render::GetSpriteSize(_sprite, _rectangle.w, _rectangle.h);

	// LogLine("--c--\tConstructed Image size of: ", sizeof(*this));
}

//! What's up with this?
Image::Image(const ScreenItem* parent_screen,	 //
			 const int& destination_width,		 //
			 const int& destination_height		 //
			 )
	: RectangleShape(parent_screen)	   //? width and height sets as disered
{
	_rectangle.w = int(_initialScaleWidth * destination_width);
	_rectangle.h = int(_initialScaleHeight * destination_height);
}

Image::Image(const ScreenItem* parent_screen,	 //
			 const char* path,					 //
			 const int& destination_width,		 //
			 const int& destination_height		 //
			 )
	: RectangleShape(parent_screen)	   //? width and height sets as disered
{
	_sprite = Render::CreateSprite(path);
	Render::SetSpriteSize(_sprite, int(_initialScaleWidth * destination_width),
						  int(_initialScaleHeight * destination_height));
	Render::GetSpriteSize(_sprite, _rectangle.w, _rectangle.h);

	// LogLine("--c--\tConstructed Image size of: ", sizeof(*this));
}

Image::~Image()
{
	Render::DestroySprite(&_sprite);

	// LogLine("--d--\tImage deconstructed...");
}

void Image::RescaleSprite(float scale_width, float scale_height)
{
	Render::SetSpriteSize(_sprite, int(scale_width * _rectangle.w), int(scale_height * _rectangle.h));
	Render::GetSpriteSize(_sprite, _rectangle.w, _rectangle.h);
}

void Image::SetIsHidden(bool new_state) { _bIsHidden = new_state; }

void Image::Render()
{
	if (!_bIsHidden)
	{
		Render::DrawSprite(_sprite, _rectangle.x, _rectangle.y);
	}
}
void Image::MoveSprite(int x, int y)
{
	_rectangle.x += x;
	_rectangle.y += y;
}

void Image::PlaceSprite(int x, int y)
{
	_rectangle.x = x;
	_rectangle.y = y;
}
