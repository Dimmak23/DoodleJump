#pragma once

class IGraphicle
{
public:
	virtual ~IGraphicle() {}

	virtual void Render() = 0;
	virtual void SetIsHidden(bool new_state) = 0;
};