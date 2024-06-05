//* Source header
#include "ParallaxItem.hpp"

//* C++ std
#include <algorithm>
#include <format>
#include <iostream>

//* Custom
//? Inheritance
#include "ScreenItem.hpp"
//? Interfaces
//? Modules
#include "Locator.hpp"
//? Wrappers
#include "SpriteWrapper.hpp"
//? Worlds
//? Utilities

ParallaxItem::ParallaxItem(				   //
	const ScreenItem* parent_screen,	   //
	const char* path,					   //
	const unsigned int& desired_width,	   //
	const unsigned int& desired_height,	   //
	const MoveVariants& variant			   //
	)
	: _Screen(parent_screen)
{
	int SidesQty = (variant == MoveVariants::BOTH) ? 4 : 2;

	//? Creating all sprites
	for (size_t index{}; index < SidesQty; index++)
	{
		_Images.push_back(std::make_unique<SpriteWrapper>(_Screen, path,	//
														  desired_width, desired_height));
	}

	//? Creating shallow locator
	_Locator = std::make_unique<Locator>(nullptr);

	//? Placing all sprites
	// TODO: implement for BOTH variant
	for (size_t index{}; index < SidesQty; index++)
	{
		if (variant == MoveVariants::UP_DOWN)
		{
			_Locator->resetBody(_Images[index].get()->getBody());
			_Locator->setLTCornerLocation(0, (index == 0) ? 0 : -_Images[index].get()->height());
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

void ParallaxItem::relocate(int delta_x, int delta_y)
{
	for (auto& Image : _Images)
	{
		_Locator->resetBody(Image.get()->getBody());
		_Locator->relocate(delta_x, delta_y);
	}

	if (_Images.empty()) return;

	// TODO: this implements only UP/DOWN movements
	//? Wait until last iamge will be at the screen top
	if ((_Images.end() - 1)->get()->top() >= 0)
	{
		//? Use locator for the first image
		_Locator->resetBody(_Images.begin()->get()->getBody());
		//? Place first image to the place which be empty soon
		_Locator->setLTCornerLocation(0, -_Images.begin()->get()->height());
		//? Finally swap images to repeat process again later
		std::iter_swap(_Images.begin(), _Images.end() - 1);
	}
}

void ParallaxItem::render()
{
	for (auto& Image : _Images)
	{
		Image->render();
	}
}
