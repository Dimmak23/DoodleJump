#pragma once

class IGraphicle
{
public:
	virtual ~IGraphicle() {}

	virtual void render() = 0;
	virtual void setIsHidden(bool new_state) = 0;
};