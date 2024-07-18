//* Source header
#include "rectangle/RectangleShape.hpp"

//* Game
#include "screen/ScreenItem.hpp"

RectangleShape::RectangleShape() : _InitialScaleWidth(1.f), _InitialScaleHeight(1.f)
{
	//? Value need to change when we will got width and height
	_centerRotation = new Point(0, 0);
	// std::cout << "_scaleFactorW: " << _scaleFactorW << ", _scaleFactorH: " << _scaleFactorH << '\n';
	// std::cout << std::format("Constructed RectangleShape size of: {}\n", sizeof(*this));
}

RectangleShape::RectangleShape(const ScreenItem* parent_screen)
	: _InitialScaleWidth(parent_screen->ScaleWidth), _InitialScaleHeight(parent_screen->ScaleHeight)
{
	_centerRotation = new Point(0, 0);
	// std::cout << std::format("Constructed RectangleShape size of: {}\n", sizeof(*this));
}

RectangleShape::~RectangleShape()
{
	delete _centerRotation;
	_centerRotation = nullptr;
}

void RectangleShape::scale(float symmetrical_scale)
{
	auto oldWidth{ _rectangle.w };
	_rectangle.w = static_cast<int>(static_cast<float>(_rectangle.w) * symmetrical_scale);
	_rectangle.x += (oldWidth - _rectangle.w) / 2;

	auto oldHeight{ _rectangle.h };
	_rectangle.h = static_cast<int>(static_cast<float>(_rectangle.h) * symmetrical_scale);
	_rectangle.y += (oldHeight - _rectangle.h) / 2;

	// std::cout << "Lx: " << _rectangle.x << ", Ly: " << _rectangle.y << '\n';
	// std::cout << "Rx: " << _rectangle.x + _rectangle.w << ", Ry: " << _rectangle.y + _rectangle.h << '\n';
}

void RectangleShape::scale(float scale_width, float scale_height)
{
	auto oldWidth{ _rectangle.w };
	_rectangle.w = static_cast<int>(static_cast<float>(_rectangle.w) * scale_width);
	_rectangle.x += (oldWidth - _rectangle.w) / 2;

	auto oldHeight{ _rectangle.h };
	_rectangle.h = static_cast<int>(static_cast<float>(_rectangle.h) * scale_height);
	_rectangle.y += (oldHeight - _rectangle.h) / 2;
}

void RectangleShape::resetCenterPoint()
{
	_centerRotation->x = _rectangle.x + _rectangle.w / 2;
	_centerRotation->y = _rectangle.y + _rectangle.h / 2;
}

double* RectangleShape::getAngle() { return &_angle; }
