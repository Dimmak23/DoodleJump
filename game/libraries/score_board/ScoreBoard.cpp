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
	: _screen(parent_screen)
	, _distanceLabelPath(distance_label_path)
	, _platformsLabelPath(platforms_label_path)
	, _digitsLabelsPath(digits_label_path)
	, _digitsQuantity(digits_qty)
{
	//? Where scaore board will be placed
	_placement = std::make_unique<Point>(right_top_corner_placement);
	//? Need to know how far player now
	_origin = std::make_unique<Point>(0, 0);
	//? This locator will place everything
	_sharedLocator = std::make_unique<Locator>(nullptr);	//? initialized as empty, later will be initialized
	//? Initialize other utilities
	Initialize();
}

ScoreBoard::~ScoreBoard() {}

void ScoreBoard::SetPlayerBodyConnection(RectangleCore* body)
{
	_playerBody = body;
	//? Place origin point using player body
	if (_playerBody && _origin)
	{
		_origin->x = _playerBody->Right();
		_origin->y = _playerBody->Bottom();
		// std::cout << std::format("Origin x: {}, y: {}.\n", _Origin->x, _Origin->y);
	}
}

void ScoreBoard::SetStaticWorldConnection(IAccounting* world) { _platformsWorld = world; }

void ScoreBoard::Initialize()
{
	// TODO: a lot of resources in the application could be initialize at the start
	// TODO: after level reload they could be simply showed up in a initial state
	// TODO: so no need to reset some pointers
	//? Load sprite for score pixels label
	_distanceLabel = std::make_unique<Image>(	 //
		_screen,								 //
		_distanceLabelPath,						 //
		Sizes::Widgets::PlatformsLabelWidth,	 //
		Sizes::Widgets::PlatformsLabelHeight	 //
	);
	_sharedLocator->ResetBody(_distanceLabel->GetBody());
	//? Placing according to design and given point
	_sharedLocator->SetLTCornerLocation(											  //
		int(_placement->x - _screen->_scaleWidth * Sizes::Widgets::LabelsOffsetX),	  //
		_placement->y																  //
	);

	//? Load sprite for score platforms lavel
	_platformsCountLabel = std::make_unique<Image>(_screen,								   //
												   _platformsLabelPath,					   //
												   Sizes::Widgets::PlatformsLabelWidth,	   //
												   Sizes::Widgets::PlatformsLabelHeight	   //
	);
	_sharedLocator->ResetBody(_platformsCountLabel->GetBody());
	//? Placing according to design and given point
	_sharedLocator->SetLTCornerLocation(											  //
		int(_placement->x - _screen->_scaleWidth * Sizes::Widgets::LabelsOffsetX),	  //
		int(_placement->y + _screen->_scaleHeight * Sizes::Widgets::LabelsOffsetY)	  //
	);

	//? Load animated sprite container for 8 digits pixels score
	for (size_t index{}; index < _distanceCountDigits; index++)
	{
		_distanceDigits.push_back(std::make_unique<AnimatedImage>(_screen,							   //
																  _digitsLabelsPath,				   //
																  _digitsQuantity,					   //
																  Sizes::Widgets::DigitsLabelWidth,	   //
																  Sizes::Widgets::DigitsLabelHeight	   //
																  ));								   //
		_sharedLocator->ResetBody(_distanceDigits.back()->GetBody());
		//? place every digit next to each other with offset
		_sharedLocator->SetLTCornerLocation(	//
			int(_placement->x - _screen->_scaleWidth * Sizes::Widgets::DigitsOffsetX +
				index * _screen->_scaleWidth * Sizes::Widgets::DigitsLabelOffsetX),	   //
			_placement->y															   //
		);
		//? Initially hiding all digits except first
		if (index)
		{
			_distanceDigits.back()->SetIsHidden(true);
		}
	}

	//? Load animated sprite container for 6 digits platforms score
	for (size_t index{}; index < _platformsCountDigits; index++)
	{
		_platformsDigits.push_back(std::make_unique<AnimatedImage>(_screen,								//
																   _digitsLabelsPath,					//
																   _digitsQuantity,						//
																   Sizes::Widgets::DigitsLabelWidth,	//
																   Sizes::Widgets::DigitsLabelHeight	//
																   ));									//
		_sharedLocator->ResetBody(_platformsDigits.back()->GetBody());
		//? place every digit next to each other with offset
		_sharedLocator->SetLTCornerLocation(	//
			int(_placement->x - _screen->_scaleWidth * Sizes::Widgets::DigitsOffsetX +
				index * _screen->_scaleWidth * Sizes::Widgets::DigitsLabelOffsetX),		  //
			int(_placement->y + _screen->_scaleHeight * Sizes::Widgets::LabelsOffsetY)	  //
		);
		//? Initially hiding all digits except first
		if (index)
		{
			_platformsDigits.back()->SetIsHidden(true);
		}
	}

	_currentPixelsCounter = 0;
	_currentPlatformsCounter = 0;
}

void ScoreBoard::Relocate(int delta_x, int delta_y)
{
	//? Relocate pixels counter origin point, because scene was moved
	_origin->x += delta_x;
	_origin->y += delta_y;
}

void ScoreBoard::Tick(float delta_t)
{
	//? Update pixels distance using player current location
	int Offset = _origin->y - _playerBody->Bottom();
	_pixelsCounter = (Offset < 0) ? 0 : Offset;
	// std::cout << std::format("_PixelsCounter: {}.\n", _PixelsCounter);

	//? Update platforms counter widget
	_platformsCounter = _platformsWorld->GetDeletedPlatformsCount() +
						_platformsWorld->GetAllBellowPlatformsCount(_playerBody->Bottom());
	// std::cout << std::format("_PlatformsCounter: {}.\n", _PlatformsCounter);

	//? If distance changed it will be updated on the widget
	UpdateWidgetsContainer(_pixelsCounter, _currentPixelsCounter, _distanceDigits);

	//? If bellow platforms changed it will be updated on the widget
	UpdateWidgetsContainer(_platformsCounter, _currentPlatformsCounter, _platformsDigits);
}

void ScoreBoard::Clear()
{
	//? Reset smart pointers
	for (auto& PlatformsDigit : _platformsDigits)
	{
		PlatformsDigit.reset();
	}
	for (auto& DistanceDigit : _distanceDigits)
	{
		DistanceDigit.reset();
	}
	_platformsCountLabel.reset();
	_distanceLabel.reset();
}

void ScoreBoard::Render()
{
	//? Render sprites here
	_distanceLabel->Render();
	_platformsCountLabel->Render();
	for (auto& DistanceDigit : _distanceDigits)
	{
		DistanceDigit->Render();
	}
	for (auto& PlatformsDigit : _platformsDigits)
	{
		PlatformsDigit->Render();
	}
}

void ScoreBoard::UpdateWidgetsContainer(const size_t& new_counter, size_t& current_counter,
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
		DistanceDigit->SetIsHidden(true);
	}

	//? Set iamges, unhide what need
	for (size_t index{}; index < NewDigits.size(); index++)
	{
		Digits[index]->SetCharacterFrame(NewDigits[NewDigits.size() - 1 - index]);
		Digits[index]->SetIsHidden(false);
	}

	//? Change current counter
	current_counter = new_counter;
}
