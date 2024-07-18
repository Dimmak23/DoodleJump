//* Source header
#include "score_board/ScoreBoard.hpp"

//* Game
#include "graphics/animated_image/AnimatedImage.hpp"
#include "graphics/image/Image.hpp"
#include "level/world/interface/I_Accounting.hpp"
#include "modules/locator/Locator.hpp"
#include "rectangle/RectangleCore.hpp"
#include "screen/ScreenItem.hpp"
#include "utilities/Sizes.hpp"

//* C++ std
#include <format>
#include <iostream>

ScoreBoard::ScoreBoard(const ScreenItem* parent_screen,			   //
					   const Point& right_top_corner_placement,	   //
					   const char* distance_label_path,			   //
					   const char* platforms_label_path,		   //
					   const char* digits_label_path,			   //
					   const unsigned int digits_qty)
	: _Screen(parent_screen)
	, _DistanceLabelPath(distance_label_path)
	, _PlatformsLabelPath(platforms_label_path)
	, _DigitsLabelsPath(digits_label_path)
	, _DigitsQuantity(digits_qty)
{
	//? Where scaore board will be placed
	_Placement = std::make_unique<Point>(right_top_corner_placement);
	//? Need to know how far player now
	_Origin = std::make_unique<Point>(0, 0);
	//? This locator will place everything
	_SharedLocator = std::make_unique<Locator>(nullptr);	//? initialized as empty, later will be initialized
	//? Initialize other utilities
	initialize();
}

ScoreBoard::~ScoreBoard() {}

void ScoreBoard::setPlayerBodyConnection(RectangleCore* body)
{
	_PlayerBody = body;
	//? Place origin point using player body
	if (_PlayerBody && _Origin)
	{
		_Origin->x = _PlayerBody->right();
		_Origin->y = _PlayerBody->bottom();
		// std::cout << std::format("Origin x: {}, y: {}.\n", _Origin->x, _Origin->y);
	}
}

void ScoreBoard::setStaticWorldConnection(IAccounting* world) { _PlatformsWorld = world; }

void ScoreBoard::initialize()
{
	// TODO: a lot of resources in the application could be initialize at the start
	// TODO: after level reload they could be simply showed up in a initial state
	// TODO: so no need to reset some pointers
	//? Load sprite for score pixels label
	_DistanceLabel = std::make_unique<Image>(	 //
		_Screen,								 //
		_DistanceLabelPath,						 //
		Sizes::Widgets::PlatformsLabelWidth,	 //
		Sizes::Widgets::PlatformsLabelHeight	 //
	);
	_SharedLocator->resetBody(_DistanceLabel->getBody());
	//? Placing according to design and given point
	_SharedLocator->setLTCornerLocation(										//
		_Placement->x - _Screen->ScaleWidth * Sizes::Widgets::LabelsOffsetX,	//
		_Placement->y															//
	);

	//? Load sprite for score platforms lavel
	_PlatformsCountLabel = std::make_unique<Image>(_Screen,								   //
												   _PlatformsLabelPath,					   //
												   Sizes::Widgets::PlatformsLabelWidth,	   //
												   Sizes::Widgets::PlatformsLabelHeight	   //
	);
	_SharedLocator->resetBody(_PlatformsCountLabel->getBody());
	//? Placing according to design and given point
	_SharedLocator->setLTCornerLocation(										//
		_Placement->x - _Screen->ScaleWidth * Sizes::Widgets::LabelsOffsetX,	//
		_Placement->y + _Screen->ScaleHeight * Sizes::Widgets::LabelsOffsetY	//
	);

	//? Load animated sprite container for 8 digits pixels score
	for (size_t index{}; index < _DistanceCountDigits; index++)
	{
		_DistanceDigits.push_back(std::make_unique<AnimatedImage>(_Screen,							   //
																  _DigitsLabelsPath,				   //
																  _DigitsQuantity,					   //
																  Sizes::Widgets::DigitsLabelWidth,	   //
																  Sizes::Widgets::DigitsLabelHeight	   //
																  ));								   //
		_SharedLocator->resetBody(_DistanceDigits.back()->getBody());
		//? place every digit next to each other with offset
		_SharedLocator->setLTCornerLocation(	//
			_Placement->x - _Screen->ScaleWidth * Sizes::Widgets::DigitsOffsetX +
				index * _Screen->ScaleWidth * Sizes::Widgets::DigitsLabelOffsetX,	 //
			_Placement->y															 //
		);
		//? Initially hiding all digits except first
		if (index)
		{
			_DistanceDigits.back()->setIsHidden(true);
		}
	}

	//? Load animated sprite container for 6 digits platforms score
	for (size_t index{}; index < _PlatformsCountDigits; index++)
	{
		_PlatformsDigits.push_back(std::make_unique<AnimatedImage>(_Screen,								//
																   _DigitsLabelsPath,					//
																   _DigitsQuantity,						//
																   Sizes::Widgets::DigitsLabelWidth,	//
																   Sizes::Widgets::DigitsLabelHeight	//
																   ));									//
		_SharedLocator->resetBody(_PlatformsDigits.back()->getBody());
		//? place every digit next to each other with offset
		_SharedLocator->setLTCornerLocation(	//
			_Placement->x - _Screen->ScaleWidth * Sizes::Widgets::DigitsOffsetX +
				index * _Screen->ScaleWidth * Sizes::Widgets::DigitsLabelOffsetX,	 //
			_Placement->y + _Screen->ScaleHeight * Sizes::Widgets::LabelsOffsetY	 //
		);
		//? Initially hiding all digits except first
		if (index)
		{
			_PlatformsDigits.back()->setIsHidden(true);
		}
	}

	_CurrentPixelsCounter = 0;
	_CurrentPlatformsCounter = 0;
}

void ScoreBoard::relocate(int delta_x, int delta_y)
{
	//? Relocate pixels counter origin point, because scene was moved
	_Origin->x += delta_x;
	_Origin->y += delta_y;
}

void ScoreBoard::tick(float delta_t)
{
	//? Update pixels distance using player current location
	int Offset = _Origin->y - _PlayerBody->bottom();
	_PixelsCounter = (Offset < 0) ? 0 : Offset;
	// std::cout << std::format("_PixelsCounter: {}.\n", _PixelsCounter);

	//? Update platforms counter widget
	_PlatformsCounter = _PlatformsWorld->getDeletedPlatformsCount() +
						_PlatformsWorld->getAllBellowPlatformsCount(_PlayerBody->bottom());
	// std::cout << std::format("_PlatformsCounter: {}.\n", _PlatformsCounter);

	//? If distance changed it will be updated on the widget
	updateWidgetsContainer(_PixelsCounter, _CurrentPixelsCounter, _DistanceDigits);

	//? If bellow platforms changed it will be updated on the widget
	updateWidgetsContainer(_PlatformsCounter, _CurrentPlatformsCounter, _PlatformsDigits);
}

void ScoreBoard::clear()
{
	//? Reset smart pointers
	for (auto& PlatformsDigit : _PlatformsDigits)
	{
		PlatformsDigit.reset();
	}
	for (auto& DistanceDigit : _DistanceDigits)
	{
		DistanceDigit.reset();
	}
	_PlatformsCountLabel.reset();
	_DistanceLabel.reset();
}

void ScoreBoard::render()
{
	//? Render sprites here
	_DistanceLabel->render();
	_PlatformsCountLabel->render();
	for (auto& DistanceDigit : _DistanceDigits)
	{
		DistanceDigit->render();
	}
	for (auto& PlatformsDigit : _PlatformsDigits)
	{
		PlatformsDigit->render();
	}
}

void ScoreBoard::updateWidgetsContainer(const size_t& new_counter, size_t& current_counter,
										std::vector<std::unique_ptr<AnimatedImage>>& Digits)
{
	if (new_counter == current_counter) return;

	//? Retrieve digits from integer
	std::vector<char> NewDigits;
	size_t NewDigitsCounter = new_counter;
	while (NewDigitsCounter)
	{
		NewDigits.push_back(NewDigitsCounter % 10);
		NewDigitsCounter /= 10;
	}

	//? Hide all for now
	for (auto& DistanceDigit : Digits)
	{
		DistanceDigit->setIsHidden(true);
	}

	//? Set iamges, unhide what need
	for (size_t index{}; index < NewDigits.size(); index++)
	{
		Digits[index]->setCharacterFrame(NewDigits[NewDigits.size() - 1 - index]);
		Digits[index]->setIsHidden(false);
	}

	//? Change current counter
	current_counter = new_counter;
}
