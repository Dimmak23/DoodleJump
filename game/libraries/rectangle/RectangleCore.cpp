//* Source header
#include "rectangle/RectangleCore.hpp"

//* C++ std

//* Custom
//? Inheritance
//? Interfaces
//? Modules
//? Wrappers
//? Worlds
//? Utilities

Rectangle* RectangleCore::getBody() { return &_rectangle; }

int RectangleCore::top() const { return _rectangle.y; }

int RectangleCore::bottom() const { return (_rectangle.y + _rectangle.h); }

int RectangleCore::left() const { return _rectangle.x; }

int RectangleCore::right() const { return _rectangle.x + _rectangle.w; }

Point RectangleCore::center() const { return { _rectangle.x + _rectangle.w / 2, _rectangle.y + _rectangle.h / 2 }; }

unsigned int RectangleCore::width() const { return _rectangle.w; }

unsigned int RectangleCore::height() const { return _rectangle.h; }

void RectangleCore::setBody(const Rectangle& new_body) { _rectangle = new_body; }
