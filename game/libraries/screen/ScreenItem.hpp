#pragma once

struct ScreenItem
{
	ScreenItem(						   //
		const unsigned int& width,	   //
		const unsigned int& height,	   //
		const float& scale_x,		   //
		const float& scale_y)
		: ApplicationWidth(width), ApplicationHeight(height), ScaleWidth(scale_x), ScaleHeight(scale_y)
	{}

	ScreenItem(const ScreenItem& right) = delete;
	ScreenItem& operator=(const ScreenItem& right) = delete;
	ScreenItem& operator=(ScreenItem&& right) = delete;

	const unsigned int ApplicationWidth{};
	const unsigned int ApplicationHeight{};
	const float ScaleWidth{};
	const float ScaleHeight{};
};
