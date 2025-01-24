#pragma once

struct ScreenItem
{
	ScreenItem(						   //
		const unsigned int& width,	   //
		const unsigned int& height,	   //
		const float& scale_x,		   //
		const float& scale_y)
		: _applicationWidth(width), _applicationHeight(height), _scaleWidth(scale_x), _scaleHeight(scale_y)
	{}

	ScreenItem(const ScreenItem& right) = delete;
	ScreenItem& operator=(const ScreenItem& right) = delete;
	ScreenItem& operator=(ScreenItem&& right) = delete;

	const unsigned int _applicationWidth{};
	const unsigned int _applicationHeight{};
	const float _scaleWidth{};
	const float _scaleHeight{};
};
