//* Source header
#include "GameApplication.hpp"

//* SDL2: Connector
#include "SDL2_connector/Launcher.hpp"
#include "SDL2_connector/Loger.hpp"

//* Game
#include "level/level/Level.hpp"
#include "rectangle/RectangleCore.hpp"
#include "utilities/Sizes.hpp"

//* C++ std
#include <format>
#include <iostream>
#include <sstream>
#include <string>

GameApplication::GameApplication(int argc, char** argv)
{
	//? Preconstruction details
	constexpr int minAppWidth{ 640 };
	constexpr int minAppHeight{ 480 };

	//? Commands, delimeter
	const char* windowedModeCommand{ "-window" };
	const char* fullscreenModeCommand{ "-fullscreen" };
	const char* screenSizeDelimeter{ "x" };

	//? Implement parsed parameters from program run
	if (argc == 3)
	{
		//? console sizes
		std::string original_window_command(windowedModeCommand);
		std::string original_fullscreen_command(fullscreenModeCommand);

		std::string parsed_command(argv[1]);
		if (original_window_command == parsed_command)
		{
			_bIsFullScreen = false;
		}
		//? fullscreen mode
		else if (original_fullscreen_command == parsed_command)
		{
			_bIsFullScreen = true;
		}
		else
		{
			LogLine("Incorrect first option, should be -fullscreen or -window.");
			FallBackToDefault(true);
			_appWidth = minAppWidth;
			_appHeight = minAppHeight;
		}

		//? can work further
		std::string parsed_size_command(argv[2]);
		size_t delimeter_position = parsed_size_command.find(screenSizeDelimeter);
		if (delimeter_position != std::string::npos)
		{
			//? can work further
			std::istringstream input_check(argv[2]);
			char parsed_delimeter;
			if (input_check >> _appWidth >> parsed_delimeter >> _appHeight &&
				parsed_delimeter == *screenSizeDelimeter && input_check.eof())
			{
				//? can work further
				if (_appWidth >= minAppWidth && _appHeight >= minAppHeight)
				{
					if (_appWidth >= Sizes::Screen::MaximumWidth)
					{
						_appWidth = Sizes::Screen::MaximumWidth;
					}
					if (_appHeight >= Sizes::Screen::MaximumHeight)
					{
						_appHeight = Sizes::Screen::MaximumHeight;
					}

					LogLine("Input is valid. Width: ", _appWidth, ", Height: ", _appHeight);
				}
				else
				{
					LogLine("[!]\t", parsed_size_command);
					LogLine("Size couldn't be negative or less then ", minAppWidth, "x", minAppHeight, ".");
					FallBackToDefault(true);
					_appWidth = minAppWidth;
					_appHeight = minAppHeight;
				}
			}
			else
			{
				LogLine(
					"Couldn't extract size from command. Possible contain forbidden letters. Correct "	  //,
					"format: 'WITHxHEIGHT'."															  //
				);
				FallBackToDefault(true);
				_appWidth = minAppWidth;
				_appHeight = minAppHeight;
			}
		}
		else
		{
			LogLine("Didn't found letter 'x' in the command. Correct format: 'WITHxHEIGHT'.");
			FallBackToDefault(true);
			_appWidth = minAppWidth;
			_appHeight = minAppHeight;
		}
	}
	else if (argc == 1)
	{
		//? default sizes with no error
		LogLine("Input is valid. Windowed mode with ", minAppWidth, "x", minAppHeight, " size.");
		FallBackToDefault(false);
		_appWidth = minAppWidth;
		_appHeight = minAppHeight;
	}
	else
	{
		//? set default but don't run
		//! error in the input
		LogLine("Too much commands. Use:\n",			   //
				"'game.exe',\n or\n",					   //
				"'game.exe -window WITHxHEIGHT'\nor\n",	   //
				"'game.exe -fullscreen WITHxHEIGHT'."	   //
		);
		FallBackToDefault(true);
		_appWidth = minAppWidth;
		_appHeight = minAppHeight;
	}

	_applicationScaleX = float(_appWidth) / float(Sizes::Screen::MaximumWidth);
	_applicationScaleY = float(_appHeight) / float(Sizes::Screen::MaximumHeight);

	//? Here pass screen details to populate inside libraries
	_level = std::make_unique<Level>(_appWidth, _appHeight, _applicationScaleX, _applicationScaleY);

	//? Invoke memory for MousePosition point
	_mousePosition = std::make_unique<Point>(0, 0);

	// std::cout << "Constructed GameApplication size of:" << sizeof(*this) << '\n';
}

GameApplication::~GameApplication() { /* std::cout << "Deleted GameApplication...\n"; */ }

void GameApplication::PreInit(int& width, int& height, bool& fullscreen)
{
	width = _appWidth;
	height = _appHeight;
	fullscreen = _bIsFullScreen;
}

bool GameApplication::Init()
{
	_level->Initialize(_bIsRunning);
	return true;
}

void GameApplication::Close() {}

bool GameApplication::Tick()
{
	if (_bIsRunning)
	{
		//& Checking keyboard

		if (_bPressedLeft)
		{
			_level->OnLeftArrowClicked();
		}
		if (_bPressedRight)
		{
			_level->OnRightArrowClicked();
		}
		if (_bPressedUp)
		{
			_level->OnUpArrowClicked();
		}
		if (_bPressedDown)
		{
			_level->OnDownArrowClicked();
		}

		if (!_bPressedLeft && !_bPressedRight)
		{
			_level->OnHorizontalArrowsReleased();
		}
		if (!_bPressedUp && !_bPressedDown)
		{
			_level->OnVerticalArrowsReleased();
		}

		_level->Tick(_deltaT, _bIsRunning);

		if (_bLeftMouseButtonClicked)
		{
			_level->OnMouseLeftButtonClick(*_mousePosition);
		}
		else
		{
			_level->OnMouseLeftButtonReleased();
		}
	}
	else
	{
		_level->Clear();					//? tested
		_level->Initialize(_bIsRunning);	//? tested

		_bLeftMouseButtonClicked = false;
	}

	float new_time_point = (float)Launcher::GetTickCount();
	_deltaT = new_time_point - _previousTimePoint;
	_previousTimePoint = new_time_point;

	return false;
}

void GameApplication::OnMouseMove(int x, int y, int xrelative, int yrelative)
{
	_mousePosition->x = x;
	_mousePosition->y = y;
}

void GameApplication::OnMouseButtonClick(IAMouseButton button, bool isReleased)
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

void GameApplication::OnKeyPressed(IAKey k)
{
	if (_level)
	{
		_level->EnablePlayerGravity(true);
	}

	switch (k)
	{
		case IAKey::UP: {
			_bPressedUp = true;
			break;
		}
		case IAKey::DOWN: {
			_bPressedDown = true;
			break;
		}
		case IAKey::RIGHT: {
			_bPressedRight = true;
			break;
		}
		case IAKey::LEFT: {
			_bPressedLeft = true;
			break;
		}
		default: {
			break;
		}
	}
}

void GameApplication::OnKeyReleased(IAKey k)
{
	switch (k)
	{
		case IAKey::UP:
		case IAKey::DOWN: {
			_bPressedUp = false;
			_bPressedDown = false;
			break;
		}
		case IAKey::RIGHT:
		case IAKey::LEFT: {
			_bPressedRight = false;
			_bPressedLeft = false;
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
