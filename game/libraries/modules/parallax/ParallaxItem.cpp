//* Source header
#include "modules/parallax/ParallaxItem.hpp"

//* Game
#include "graphics/image/Image.hpp"
#include "modules/locator/Locator.hpp"
#include "screen/ScreenItem.hpp"

//* C++ std
#include <algorithm>
#include <format>
#include <iostream>

ParallaxItem::ParallaxItem(				   //
	const ScreenItem* parent_screen,	   //
	const char* path,					   //
	const unsigned int& desired_width,	   //
	const unsigned int& desired_height,	   //
	const MoveVariants& variant			   //
	)
	: _screen(parent_screen)
{
	int SidesQty = (variant == MoveVariants::BOTH) ? 4 : 2;

	//? Creating all sprites
	for (size_t index{}; index < SidesQty; index++)
	{
		_images.push_back(std::make_unique<Image>(_screen, path,	//
												  desired_width, desired_height));
	}

	//? Creating shallow locator
	_locator = std::make_unique<Locator>(nullptr);

	//? Placing all sprites
	// TODO: implement for BOTH variant
	for (size_t index{}; index < SidesQty; index++)
	{
		if (variant == MoveVariants::UP_DOWN)
		{
			_locator->ResetBody(_images[index].get()->GetBody());
			_locator->SetLTCornerLocation(0, (index == 0) ? 0 : -int(_images[index].get()->Height()));
			// std::cout << std::format("Placed image for parallax... x: {}, y: {}.\n", _Locator->getX(),
			// 						 _Locator->getY());
		}
		else if (variant == MoveVariants::LEFT_RIGHT)
		{
			// TODO: implement for LEFT_RIGHT variant
		}
	}
}

ParallaxItem::~ParallaxItem() {}

void ParallaxItem::Relocate(int delta_x, int delta_y)
{
	for (auto& Image : _images)
	{
		_locator->ResetBody(Image.get()->GetBody());
		_locator->Relocate(delta_x, delta_y);
	}

	if (_images.empty()) return;

	// TODO: this implements only UP/DOWN movements
	//? Wait until last iamge will be at the screen top
	if ((_images.end() - 1)->get()->Top() >= 0)
	{
		//? Use locator for the first image
		_locator->ResetBody(_images.begin()->get()->GetBody());
		//? Place first image to the place which be empty soon
		_locator->SetLTCornerLocation(0, -int(_images.begin()->get()->Height()));
		//? Finally swap images to repeat process again later
		std::iter_swap(_images.begin(), _images.end() - 1);
	}
}

void ParallaxItem::Render()
{
	for (auto& Image : _images)
	{
		Image->Render();
	}
}
