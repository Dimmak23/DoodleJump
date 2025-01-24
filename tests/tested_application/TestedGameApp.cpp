//* Source header
#include "TestedGameApp.hpp"

//* SDL2: Connector
#include "SDL2_Connector/Launcher.hpp"
#include "SDL2_Connector/Loger.hpp"

//* twoDarKade: framework
#include "graphics/image/Image.hpp"
#include "screen/ScreenItem.hpp"

//* Custom

//* C++ std
#include <sstream>

TestedGameApp::TestedGameApp(int argc, char** argv)
{
	//? Implement parsed parameters from program run
	if (argc == 3)
	{
		//? console sizes
		std::string original_window_command(_windowedModeCommand);
		std::string original_fullscreen_command(_fullscreenModeCommand);

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
		}

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
					LogLine("Input is valid. Width: ", _appWidth, ", Height: ", _appHeight);
				}
				else
				{
					LogLine("[!]\t", parsed_size_command);
					LogLine("Size couldn't be negative or less then ", _minAppWidth, "x", _minAppHeight, ".");
					FallBackToDefault(true);
				}
			}
			else
			{
				LogLine(
					"Couldn't extract size from command. Possible contain forbidden letters. Correct "	  //,
					"format: 'WITHxHEIGHT'."															  //
				);
				FallBackToDefault(true);
			}
		}
		else
		{
			LogLine("Didn't found letter 'x' in the command. Correct format: 'WITHxHEIGHT'.");
			FallBackToDefault(true);
		}
	}
	else if ((argc == 2) && (argc == 1))
	{
		//? default sizes with no error
		LogLine("Input is valid. Windowed mode with ", _minAppWidth, "x", _minAppHeight, " size.");
		FallBackToDefault(false);
	}
	else
	{
		//? set default but don't run
		//! error in the input
		LogLine("Too much commands. Use 'DoodleJump.exe', ",	//
				"'game -window WITHxHEIGHT' or ",				//
				"'game -fullscreen WITHxHEIGHT' .");
		FallBackToDefault(true);
	}

	_applicationScaleX = float(_appWidth) / float(_maxAppWidth);
	_applicationScaleY = float(_appHeight) / float(_maxAppHeight);

	_Screen = std::make_unique<ScreenItem>(_appWidth, _appHeight, _applicationScaleX, _applicationScaleY);

	LogLine("--c--\tConstructed TestedGameApp size of: ", sizeof(*this));
}

TestedGameApp::~TestedGameApp() { LogLine("--d--\tDeleted TestedGameApp..."); }

void TestedGameApp::PreInit(int& width, int& height, bool& fullscreen)
{
	width = _appWidth;
	height = _appHeight;
	fullscreen = _bIsFullScreen;
}

bool TestedGameApp::Init()
{
	//! TESTING
	_TestedImage = std::make_unique<Image>(_Screen.get(), "assets/images/space-right@2x.png", 200, 200);
	timer = 500.f;
	//! TESTING

	return true;
}

void TestedGameApp::Close() {}

bool TestedGameApp::Tick()
{
	//! TESTING
	_TestedImage->Render();
	if (timer <= 0)
	{
		// _TestedImage->moveSprite(10, 10);
		timer = 500.f;
	}
	CurrentTime = Launcher::GetTickCount();
	_deltaT = static_cast<float>(CurrentTime - PreviousTime);
	PreviousTime = CurrentTime;
	timer -= _deltaT;
	//! TESTING

	return false;
}

void TestedGameApp::OnMouseMove(int x, int y, int xrelative, int yrelative)
{
	xMousePos = x;
	yMousePos = y;
	// _TestedImage->placeSprite(x, y);
	LogLine("xrel: ", xrelative, ", yrel: ", yrelative);
}

void TestedGameApp::OnMouseButtonClick(IAMouseButton button, bool isReleased)
{
	if (isReleased)
	{
		LogLine("Mouse button released index: ", static_cast<int>(button));
		_TestedImage->PlaceSprite(xMousePos, yMousePos);
	}
	else
	{
		LogLine("Mouse button clicked index: ", static_cast<int>(button));
	}
}

void TestedGameApp::OnKeyPressed(IAKey k)
{
	LogLine("Pressed key: ", static_cast<int>(k));
	switch (k)
	{
		case IAKey::LEFT:
			_TestedImage->MoveSprite(-10, 0);
			break;
		case IAKey::RIGHT:
			_TestedImage->MoveSprite(10, 0);
			break;
		case IAKey::UP:
			_TestedImage->MoveSprite(0, -10);
			break;
		case IAKey::DOWN:
			_TestedImage->MoveSprite(0, 10);
			break;
		default:
			break;
	}
}

void TestedGameApp::OnKeyReleased(IAKey k) { LogLine("Released key: ", static_cast<int>(k)); }

const char* TestedGameApp::GetTitle() { return "Testing... FEB, 2024."; }

bool TestedGameApp::GetIsConstructWithError() const { return _bIsConstructedWithError; }

void TestedGameApp::FallBackToDefault(bool bIsErrorInput)
{
	_appWidth = _minAppWidth;
	_appHeight = _minAppHeight;
	_bIsConstructedWithError = bIsErrorInput;
}
