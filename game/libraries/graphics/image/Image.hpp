#pragma once

//* Game
#include "graphics/interface/IGraphicle.hpp"
#include "rectangle/RectangleShape.hpp"

//* C++ std
#include <memory>

//* Forward declarations
class Sprite;

class Image : public RectangleShape, public IGraphicle
{
public:
	//? Wrap sprite as it is
	Image(const char* path);
	//? Wrap sprite and scale up to application width
	Image(const char* path, const int& application_width);
	//? Prepare shallow for Animated Sprite container
	Image(const ScreenItem* parent_screen, const int& destination_width, const int& destination_height);
	//? Wrap sprite and scale to desired size against to application scale
	Image(const ScreenItem* parent_screen, const char* path, const int& destination_width,
		  const int& destination_height);
	virtual ~Image();

	//@ API

	//* Setters
	void RescaleSprite(float scale_width, float scale_height);
	virtual void SetIsHidden(bool new_state) override;

	//* Manipulators
	virtual void Render() override;
	void MoveSprite(int x, int y);
	void PlaceSprite(int x, int y);

private:
	//@ Methods

	//* Let's prevent copying of the wrapper
	Image(const Image&) = delete;
	Image& operator=(const Image&) = delete;

protected:
	//@ Members

	Sprite* _sprite{ nullptr };
	bool _bIsHidden{ false };
};
