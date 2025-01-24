#pragma once

//* SDL2: Connector
#include "SDL2_connector/IApplication.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class Level;
struct Point;

class GameApplication : public IApplication
{
public:
	//? Game can be created only with parameters
	GameApplication() = delete;
	GameApplication(int argc, char** argv);
	//? Destroy resources here
	virtual ~GameApplication();

	//* API for main translation unit

	//? no function calls are available here, this function should only return width, height and fullscreen values
	virtual void PreInit(int& width, int& height, bool& fullscreen) override;

	//? return : true - ok, false - failed, application will exit
	virtual bool Init() override;

	virtual void Close() override;

	//? return value: if true will exit the application
	virtual bool Tick() override;

	//? param: xrel, yrel: The relative motion in the X/Y direction
	//? param: x, y : coordinate, relative to window
	virtual void OnMouseMove(int x, int y, int xrelative, int yrelative) override;

	virtual void OnMouseButtonClick(IAMouseButton button, bool isReleased) override;

	virtual void OnKeyPressed(IAKey k) override;

	virtual void OnKeyReleased(IAKey k) override;

	virtual const char* GetTitle() override;

	//* Custom

	bool GetIsConstructWithError() const;

private:
	//@ Methods

	//* Let's prevent copying GameApplication object
	GameApplication(const GameApplication&) = delete;
	GameApplication& operator=(const GameApplication&) = delete;

	void FallBackToDefault(bool bIsErrorInput);

	//@ Members

	//* Levels
	std::unique_ptr<Level> _level{ nullptr };

	//* Mouse
	std::unique_ptr<Point> _mousePosition;

	//* Commands, delimeter
	const char* _windowedModeCommand{ "-window" };
	const char* _fullscreenModeCommand{ "-fullscreen" };
	const char* _screenSizeDelimeter{ "x" };

	//* Utilities
	int _minAppWidth{ 400 };
	int _minAppHeight{ 300 };
	int _appWidth{};
	int _appHeight{};
	float _applicationScaleX{};
	float _applicationScaleY{};
	float _previousTimePoint{};
	float _deltaT{};

	//* States
	bool _bIsConstructedWithError{ false };
	bool _bPressedLeft{ false };
	bool _bPressedRight{ false };
	bool _bPressedUp{ false };
	bool _bPressedDown{ false };
	bool _bIsFullScreen{ false };
	bool _bIsRunning{ false };
	bool _bLeftMouseButtonClicked{ false };
};
