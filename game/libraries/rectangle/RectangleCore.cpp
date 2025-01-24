//* Source header
#include "rectangle/RectangleCore.hpp"

Rectangle* RectangleCore::GetBody() { return &_rectangle; }

int RectangleCore::Top() const { return _rectangle.y; }

int RectangleCore::Bottom() const { return (_rectangle.y + _rectangle.h); }

int RectangleCore::Left() const { return _rectangle.x; }

int RectangleCore::Right() const { return _rectangle.x + _rectangle.w; }

Point RectangleCore::Center() const { return { _rectangle.x + _rectangle.w / 2, _rectangle.y + _rectangle.h / 2 }; }

unsigned int RectangleCore::Width() const { return _rectangle.w; }

unsigned int RectangleCore::Height() const { return _rectangle.h; }

void RectangleCore::SetBody(const Rectangle& new_body) { _rectangle = new_body; }
