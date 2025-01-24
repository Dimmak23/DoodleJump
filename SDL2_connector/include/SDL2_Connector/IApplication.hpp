#pragma once

enum class IAKey : int
{
	RIGHT,
	LEFT,
	DOWN,
	UP,
	COUNT
};

enum class IAMouseButton : int
{
	LEFT,
	MIDDLE,
	RIGHT,
	COUNT
};

struct IARectangle
{
	int x{};
	int y{};
	int w{};
	int h{};
};

class IApplication
{
public:
	//? no function calls are available here, this function should only return width, height and fullscreen values
	virtual void PreInit(int& width, int& height, bool& fullscreen) = 0;

	//? return : true - ok, false - failed, application will exit
	virtual bool Init() = 0;

	virtual void Close() = 0;

	//? return value: if true will exit the application
	virtual bool Tick() = 0;

	//? param: xrel, yrel: The relative motion in the X/Y direction
	//? param: x, y : coordinate, relative to window
	virtual void OnMouseMove(int x, int y, int xrelative, int yrelative) = 0;

	virtual void OnMouseButtonClick(IAMouseButton button, bool isReleased) = 0;

	virtual void OnKeyPressed(IAKey k) = 0;

	virtual void OnKeyReleased(IAKey k) = 0;

	virtual const char* GetTitle() = 0;

	virtual ~IApplication() {};
};
