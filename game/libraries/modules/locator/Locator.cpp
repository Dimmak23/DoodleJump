//* Source header
#include "modules/locator/Locator.hpp"

//* Game
#include "modules/physics_engine/I_Connectable.hpp"
#include "rectangle/RectangleCore.hpp"

//* C++ std
#include <format>
#include <iostream>

Locator::Locator(RectangleCore* rectangle) : _host((rectangle) ? rectangle->getBody() : nullptr)
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

Locator* Locator::parseBody(Rectangle* body)
{
	_host = body;
	return this;
}

void Locator::resetBody(Rectangle* body) { _host = body; }

void Locator::clearBody() { _host = nullptr; }

void Locator::setPhysicsEngineConnection(IConnectable* physics_engine) { _PhysicsEngine = physics_engine; }

void Locator::setLoggingEnabled(bool new_state) { _bLoggingEnabled = new_state; }

// void Locator::setUpdatePreciseCoordinatesCallBack(ConnectCallBackType function)
// {
// 	_UpdatePreciseCoordinates = function;
// }

void Locator::setCenterLocation(const int& x, const int& y)
{
	_host->x = x - _host->w / 2;
	_host->y = y - _host->h / 2;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setCenterLocation(Point coordinate)
{
	_host->x = coordinate.x - _host->w / 2;
	_host->y = coordinate.y - _host->h / 2;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setCenterLocation(Coordinate coordinate)
{
	_host->x = coordinate.x - _host->w / 2;
	_host->y = coordinate.y - _host->h / 2;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setTopCLocation(const int& x, const int& y)
{
	if (int temp = (x - _host->w / 2); temp < 0)
	{
		_host->x = 0;
	}
	else
		_host->x = temp;
	_host->y = y;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setBottomCLocation(const int& x, const int& y)
{
	_host->x = x - _host->w / 2;
	_host->y = y - _host->h;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setLeftCLocation(const int& x, const int& y)
{
	_host->x = x;
	_host->y = y - _host->h / 2;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setRightCLocation(const int& x, const int& y)
{
	_host->x = x - _host->w;
	_host->y = y - _host->h / 2;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setLTCornerLocation(const int& x, const int& y)
{
	_host->x = x;
	_host->y = y;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setLBCornerLocation(const int& x, const int& y)
{
	_host->x = x;
	_host->y = y - _host->h;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setRTCornerLocation(const int& x, const int& y)
{
	_host->x = x - _host->w;
	_host->y = y;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setRBCornerLocation(const int& x, const int& y)
{
	_host->x = x - _host->w;
	_host->y = y - _host->h;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setLeft(const int& x)
{
	_host->x = x;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::setTop(const int& y)
{
	_host->y = y;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

void Locator::alignYAxis(const int& x)
{
	_host->x = x - _host->w / 2;
	onPhisicsEngineUpdate(_host->x, _host->y);
}

//  int Locator::getTop() const { return _host->y; }

//  int Locator::getBottom() const { return (_host->y + _host->h); }

//  int Locator::getleft() const { return _host->x; }

//  int Locator::getRight() const { return _host->x + _host->w; }

Locator::Coordinate Locator::getTopCLocation() const { return Coordinate(_host->x + _host->w / 2, _host->y); }

Locator::Coordinate Locator::getCenter() const { return Coordinate(_host->x + _host->w / 2, _host->y + _host->h / 2); }

int Locator::getCenterX() const { return _host->x + _host->w / 2; }

int Locator::getCenterY() const { return _host->y + _host->h / 2; }

int Locator::getX() const { return _host->x; }

int Locator::getY() const { return _host->y; }

void Locator::relocate(int deltaX, int deltaY)
{
	_host->x += deltaX;
	_host->y += deltaY;
	// onPhisicsEngineUpdate(_host->x, _host->y);
	if (_PhysicsEngine)
	{
		_PhysicsEngine->moveByCamera(Point{ deltaX, deltaY });
	}
}

void Locator::onPhisicsEngineUpdate(double x, double y)
{
	if (_PhysicsEngine)
	{
		_PhysicsEngine->setPreciseCoordinate(int(x), int(y));
	}
}
