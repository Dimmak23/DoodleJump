#pragma once

//* SDL2: Connector
#include "SDL2_Connector/IApplication.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
struct ScreenItem;
class Image;

class TestedGameApp : public IApplication
{
public:
	//? Game can be created only with parameters
	TestedGameApp() = delete;
	TestedGameApp(int argc, char** argv);
	//? Destroy resources here
	virtual ~TestedGameApp();

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
	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) override;

	virtual void onMouseButtonClick(IAMouseButton button, bool isReleased) override;

	virtual void onKeyPressed(IAKey k) override;

	virtual void onKeyReleased(IAKey k) override;

	virtual const char* GetTitle() override;

	//* Custom

	bool GetIsConstructWithError() const;

private:
	//@ Methods

	//* Let's prevent copying TestedGameApp object
	TestedGameApp(const TestedGameApp&) = delete;
	TestedGameApp& operator=(const TestedGameApp&) = delete;

	void FallBackToDefault(bool bIsErrorInput);

	//@ Members

	//* States
	bool _bIsConstructedWithError{ false };
	bool _pressedLeft{ false };
	bool _pressedRight{ false };
	bool _pressedUp{ false };
	bool _pressedDown{ false };
	bool _bIsFullScreen{ false };
	bool _bIsRunning{ false };
	bool _bLeftMouseButtonClicked{ false };

	//* Commands, delimeter
	const char* _windowedModeCommand{ "-window" };
	const char* _fullscreenModeCommand{ "-fullscreen" };
	const char* _screenSizeDelimeter{ "x" };

	//* Utilities
	int _maxAppWidth{ 2560 };
	int _maxAppHeight{ 1440 };
	int _minAppWidth{ 640 };
	int _minAppHeight{ 480 };
	int _appWidth{};
	int _appHeight{};
	//! TESTING
	int xMousePos{};
	int yMousePos{};
	//! TESTING

	//* Tickers
	float _previousTimePoint{};
	float _deltaT{};
	//! TESTING
	float timer{};
	//! TESTING

	//* Utilities
	float _applicationScaleX{};
	float _applicationScaleY{};

	//! TESTING
	//* Screen
	std::unique_ptr<ScreenItem> _Screen{ nullptr };
	//* Sprite
	std::unique_ptr<Image> _TestedImage{ nullptr };
	uint64_t CurrentTime{};
	uint64_t PreviousTime{};
	//! TESTING
};
