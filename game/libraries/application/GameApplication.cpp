//* Source header
#include "GameApplication.hpp"

//* Vendor connection
//? Launch
#include "Launcher.hpp"

//* C++ std
#include <format>
#include <iostream>
#include <sstream>
#include <string>

//* Custom
//? Inheritance
#include "RectangleCore.hpp"
//? Interfaces
//? Modules
//? Wrappers
//? Worlds
//? Levels
#include "Level.hpp"
//? Utilities
#include "Sizes.hpp"

GameApplication::GameApplication(int argc, char** argv)
{
	//? Implement parsed parameters from program run
	if (argc == 3)
	{
		//? console sizes
		std::string original_window_command(_windowedModeCommand);
		std::string parsed_window_command(argv[1]);
		if (original_window_command == parsed_window_command)
		{
			//? can work further
			std::string parsed_size_command(argv[2]);
			size_t delimeter_position = parsed_size_command.find(_screenSizeDelimeter);
			if (delimeter_position != std::string::npos)
			{
				//? can work further
				std::istringstream input_check(argv[2]);
				char parsed_delimeter;
				if (input_check >> _appWidth >> parsed_delimeter >> _appHeight &&
					parsed_delimeter == *_screenSizeDelimeter && input_check.eof())
				{
					//? can work further
					if (_appWidth >= _minAppWidth && _appHeight >= _minAppHeight)
					{
						std::cout << "Input is valid. Width: " << _appWidth << ", Height: " << _appHeight << ".\n";
					}
					else
					{
						std::cout << "[!]\t" << parsed_size_command << '\n';
						std::cout << "Window size couldn't be negative or less then 400x300.\n";
						FallBackToDefault(true);
					}
				}
				else
				{
					std::cout << "Couldn't extract size from command. Possible contain forbidden letters. Correct "
								 "format: 'WITHxHEIGHT'.\n";
					FallBackToDefault(true);
				}
			}
			else
			{
				std::cout << "Didn't found letter 'x' in the command. Correct format: 'WITHxHEIGHT'.\n";
				FallBackToDefault(true);
			}
		}
		else
		{
			std::cout << "Incorrect windowed mode command text. Correct text: '-window'.\n";
			FallBackToDefault(true);
		}
	}
	else if (argc == 2)
	{
		//? fullscreen mode
		std::string original_command(_fullscreenModeCommand);
		std::string parsed_command(argv[1]);
		if (original_command == parsed_command)
		{
			_bIsFullScreen = true;
			Launcher::GetScreenSize(_appWidth, _appHeight);
			std::cout << "getScreenSize... _appWidth: " << _appWidth << ", _appHeight: " << _appHeight << ".\n";
			//! Don't work, possible bug in Dragon lake framework
			//! output was _appWidth: 927843248, _appHeight: 553.
			//! numbers changes but width always some high random number, and height some unrealistic small
			_appWidth = Sizes::Screen::MaximumWidth;
			_appHeight = Sizes::Screen::MaximumHeight;
			std::cout << "Input is valid. Fullscreen mode. Resolution width: " << _appWidth
					  << ", resolution height: " << _appHeight << ".\n";
		}
		else
		{
			std::cout << "Incorrect fullscreen mode command text. Correct text: '-fullscreen'.\n";
			FallBackToDefault(true);
		}
	}
	else if (argc == 1)
	{
		//? default sizes with no error
		std::cout << "Input is valid. Windowed mode with 400x300 size.\n";
		FallBackToDefault(false);
	}
	else
	{
		//? set default but don't run
		//! error in the input
		std::cout << "Too much commands. Use 'DoodleJump.exe', "
				  << "'DoodleJump.exe -window WITHxHEIGHT' or "
				  << "'DoodleJump.exe  -fullscreen' .\n";
		FallBackToDefault(true);
	}

	_applicationScaleX = float(_appWidth) / float(Sizes::Screen::MaximumWidth);
	_applicationScaleY = float(_appHeight) / float(Sizes::Screen::MaximumHeight);

	//? Here pass screen details to populate inside libraries
	_Level = std::make_unique<Level>(_appWidth, _appHeight, _applicationScaleX, _applicationScaleY);

	//? Invoke memory for MousePosition point
	_MousePosition = std::make_unique<Point>(0, 0);

	// std::cout << "Constructed GameApplication size of:" << sizeof(*this) << '\n';
}

GameApplication::~GameApplication()
{ /* std::cout << "Deleted GameApplication...\n"; */
}

void GameApplication::PreInit(int& width, int& height, bool& fullscreen)
{
	width = _appWidth;
	height = _appHeight;
	fullscreen = _bIsFullScreen;
}

bool GameApplication::Init()
{
	_Level->Initialize(_bIsRunning);

	//! DragonLake framework bug: function don't working
	// showCursor(true);
	// std::cout << std::format("Constructed GameApplication size of: {}\n", sizeof(*this));
	return true;
}

void GameApplication::Close() {}

bool GameApplication::Tick()
{
	if (_bIsRunning)
	{
		//& Checking keyboard

		if (_pressedLeft)
		{
			_Level->onLeftArrowClicked();
		}
		if (_pressedRight)
		{
			_Level->onRightArrowClicked();
		}
		if (_pressedUp)
		{
			_Level->onUpArrowClicked();
		}
		if (_pressedDown)
		{
			_Level->onDownArrowClicked();
		}

		if (!_pressedLeft && !_pressedRight)
		{
			_Level->onHorizontalArrowsReleased();
		}
		if (!_pressedUp && !_pressedDown)
		{
			_Level->onVerticalArrowsReleased();
		}

		_Level->Tick(_deltaT, _bIsRunning);

		if (_bLeftMouseButtonClicked)
		{
			_Level->onMouseLeftButtonClick(*_MousePosition);
		}
		else
		{
			_Level->onMouseLeftButtonReleased();
		}
	}
	else
	{
		_Level->Clear();					//? tested
		_Level->Initialize(_bIsRunning);	//? tested

		_bLeftMouseButtonClicked = false;
	}

	float new_time_point = (float)Launcher::GetTickCount();
	_deltaT = new_time_point - _previousTimePoint;
	_previousTimePoint = new_time_point;

	return false;
}

void GameApplication::onMouseMove(int x, int y, int xrelative, int yrelative)
{
	_MousePosition->x = x;
	_MousePosition->y = y;
}

void GameApplication::onMouseButtonClick(IAMouseButton button, bool isReleased)
{
	switch (button)
	{
		case IAMouseButton::LEFT: {
			if (!isReleased)
			{
				_bLeftMouseButtonClicked = true;
			}
			else
			{
				//? If Level give signal of process is done - you can set back to false
				_bLeftMouseButtonClicked = false;
			}
			break;
		}
		case IAMouseButton::MIDDLE: {
			break;
		}
		case IAMouseButton::RIGHT: {
			break;
		}
		default:
			break;
	}
}

void GameApplication::onKeyPressed(IAKey k)
{
	if (_Level)
	{
		_Level->enablePlayerGravity(true);
	}

	switch (k)
	{
		case IAKey::UP: {
			_pressedUp = true;
			break;
		}
		case IAKey::DOWN: {
			_pressedDown = true;
			break;
		}
		case IAKey::RIGHT: {
			_pressedRight = true;
			break;
		}
		case IAKey::LEFT: {
			_pressedLeft = true;
			break;
		}
		default: {
			break;
		}
	}
}

void GameApplication::onKeyReleased(IAKey k)
{
	switch (k)
	{
		case IAKey::UP:
		case IAKey::DOWN: {
			_pressedUp = false;
			_pressedDown = false;
			break;
		}
		case IAKey::RIGHT:
		case IAKey::LEFT: {
			_pressedRight = false;
			_pressedLeft = false;
			break;
		}

		default: {
			break;
		}
	}
}

const char* GameApplication::GetTitle() { return "Doodle Jump, produced by DimmaK, january 2024."; }

bool GameApplication::GetIsConstructWithError() const { return _bIsConstructedWithError; }

void GameApplication::FallBackToDefault(bool bIsErrorInput)
{
	_appWidth = _minAppWidth;
	_appHeight = _minAppHeight;
	_bIsConstructedWithError = bIsErrorInput;
}
