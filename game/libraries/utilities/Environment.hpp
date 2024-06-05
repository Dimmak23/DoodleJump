#pragma once

struct Position
{
	int x{};
	int y{};
};

namespace Environment::Platform
{
	const unsigned int InitialQty{ 16 };
	const unsigned int BottomBorderQty{ 8 };

	const Position Level_01_Positions[InitialQty]{
		{ 0, 1440 },	   // 0
		{ 320, 1440 },	   // 1
		{ 640, 1440 },	   // 2
		{ 960, 1440 },	   // 3
		{ 1280, 1440 },	   // 4
		{ 1600, 1440 },	   // 5
		{ 1920, 1440 },	   // 6
		{ 2240, 1440 },	   // 7
		{ 480, 1080 },	   // 8
		{ 880, 800 },	   // 9
		{ 400, 520 },	   // 10
		{ 0, 320 },		   // 11
		{ 760, 120 },	   // 12
		{ 1120, -40 },	   // 13
		{ 1400, -200 },	   // 14
		{ 920, -440 }	   // 15
	};

	//? Left-Bottom corner positions on the map
	const Position Level_01_BottomBorder[BottomBorderQty]{
		{ 0, 1440 },	   // 0
		{ 320, 1440 },	   // 1
		{ 640, 1440 },	   // 2
		{ 960, 1440 },	   // 3
		{ 1280, 1440 },	   // 4
		{ 1600, 1440 },	   // 5
		{ 1920, 1440 },	   // 6
		{ 2240, 1440 }	   // 7
	};
};	  // namespace Environment::Platform
