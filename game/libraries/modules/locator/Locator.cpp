//* Source header
#include "modules/locator/Locator.hpp"

//* Game
#include "modules/physics_engine/I_Connectable.hpp"
#include "rectangle/RectangleCore.hpp"

//* C++ std
#include <format>
#include <iostream>

Locator::Locator(RectangleCore* rectangle) : _host((rectangle) ? rectangle->GetBody() : nullptr)
{
	// std::cout << "Locator received host with x: " << _host->x << ", y: " << _host->y << ", w: " << _host->w
	//   << ", h: " << _host->h << '\n';
	// std::cout << "Constructed Locator size of:" << sizeof(*this) << '\n';
}

Locator::~Locator()
{
	// delete _host;	 //! can't do this, it's already deleted
	_host = nullptr;
	// std::cout << "Deleted Locator...\n";
}

Locator* Locator::ParseBody(Rectangle* body)
{
	_host = body;
	return this;
}

void Locator::ResetBody(Rectangle* body) { _host = body; }

void Locator::ClearBody() { _host = nullptr; }

void Locator::SetPhysicsEngineConnection(IConnectable* physics_engine) { _physicsEngine = physics_engine; }

void Locator::SetLoggingEnabled(bool new_state) { _bLoggingEnabled = new_state; }

// void Locator::setUpdatePreciseCoordinatesCallBack(ConnectCallBackType function)
// {
// 	_updatePreciseCoordinates = function;
// }

void Locator::SetCenterLocation(const int& x, const int& y)
{
	_host->x = x - _host->w / 2;
	_host->y = y - _host->h / 2;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetCenterLocation(Point coordinate)
{
	_host->x = coordinate.x - _host->w / 2;
	_host->y = coordinate.y - _host->h / 2;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetCenterLocation(Coordinate coordinate)
{
	_host->x = coordinate.x - _host->w / 2;
	_host->y = coordinate.y - _host->h / 2;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetTopCLocation(const int& x, const int& y)
{
	if (int temp = (x - _host->w / 2); temp < 0)
	{
		_host->x = 0;
	}
	else
		_host->x = temp;
	_host->y = y;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetBottomCLocation(const int& x, const int& y)
{
	_host->x = x - _host->w / 2;
	_host->y = y - _host->h;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetLeftCLocation(const int& x, const int& y)
{
	_host->x = x;
	_host->y = y - _host->h / 2;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetRightCLocation(const int& x, const int& y)
{
	_host->x = x - _host->w;
	_host->y = y - _host->h / 2;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetLTCornerLocation(const int& x, const int& y)
{
	_host->x = x;
	_host->y = y;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetLBCornerLocation(const int& x, const int& y)
{
	_host->x = x;
	_host->y = y - _host->h;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetRTCornerLocation(const int& x, const int& y)
{
	_host->x = x - _host->w;
	_host->y = y;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetRBCornerLocation(const int& x, const int& y)
{
	_host->x = x - _host->w;
	_host->y = y - _host->h;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetLeft(const int& x)
{
	_host->x = x;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::SetTop(const int& y)
{
	_host->y = y;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::AlignYAxis(const int& x)
{
	_host->x = x - _host->w / 2;
	OnPhisicsEngineUpdate(_host->x, _host->y);
}

//  int Locator::getTop() const { return _host->y; }

//  int Locator::getBottom() const { return (_host->y + _host->h); }

//  int Locator::getleft() const { return _host->x; }

//  int Locator::getRight() const { return _host->x + _host->w; }

Locator::Coordinate Locator::GetTopCLocation() const { return Coordinate(_host->x + _host->w / 2, _host->y); }

Locator::Coordinate Locator::GetCenter() const { return Coordinate(_host->x + _host->w / 2, _host->y + _host->h / 2); }

int Locator::GetCenterX() const { return _host->x + _host->w / 2; }

int Locator::GetCenterY() const { return _host->y + _host->h / 2; }

int Locator::GetX() const { return _host->x; }

int Locator::GetY() const { return _host->y; }

void Locator::Relocate(int deltaX, int deltaY)
{
	_host->x += deltaX;
	_host->y += deltaY;
	// onPhisicsEngineUpdate(_host->x, _host->y);
	if (_physicsEngine)
	{
		_physicsEngine->MoveByCamera(Point{ deltaX, deltaY });
	}
}

void Locator::OnPhisicsEngineUpdate(double x, double y)
{
	if (_physicsEngine)
	{
		_physicsEngine->SetPreciseCoordinate(int(x), int(y));
	}
}
