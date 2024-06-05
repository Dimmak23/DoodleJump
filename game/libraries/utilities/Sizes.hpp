#pragma once

//@ All sizes designed for screen size 2560x1440
//@ in any other case sizes would be adjusted

namespace Sizes::Screen
{
	const int MaximumWidth{ 2560 };
	const int MaximumHeight{ 1440 };
};	  // namespace Sizes::Screen

namespace Sizes::Background
{
	const int DefaultWidth{ 2560 };		// 640
	const int DefaultHeight{ 4096 };	// 1024
};										// namespace Sizes::Background

namespace Sizes::Player
{
	const int DefaultWidth{ 124 };
	const int DefaultHeight{ 120 };
	const int DefaultSpawnPointHeight{ 60 };
};	  // namespace Sizes::Player

namespace Sizes::Camera
{
	const int DefaultWidth{ 2567 };
	const int DefaultHeight{ 328 };
};	  // namespace Sizes::Camera

namespace Sizes::Environment
{
	const int PlatformWidth{ 270 };
	const int PlatformHeight{ 27 };
	const int PlatformExplodedWidth_01{ 116 };
	const int PlatformExplodedHeight_01{ 36 };
	const int PlatformExplodedWidth_02{ 122 };
	const int PlatformExplodedHeight_02{ 51 };
	const int PlatformExplodedWidth_03{ 100 };
	const int PlatformExplodedHeight_03{ 54 };

	const int EnemyWidth[5]{ 97, 163, 166, 162, 160 };
	const int EnemyHeight[5]{ 97, 104, 108, 103, 108 };

	const int WormholeWidth{ 140 };
	const int WormholeHeight{ 140 };
};	  // namespace Sizes::Environment

namespace Sizes::Projectiles
{
	const int AmmoWidth{ 32 };
	const int AmmoHeight{ 32 };
};	  // namespace Sizes::Projectiles

namespace Sizes::Widgets
{
	const int LabelsOffsetY{ 100 };
	const int LabelsOffsetX{ 410 };		   // 510
	const int DigitsOffsetX{ 190 };		   // 290
	const int DigitsLabelOffsetX{ 24 };	   // 36
	const int PlatformsLabelWidth{ 211 };
	const int PlatformsLabelHeight{ 74 };
	const int DigitsLabelWidth{ 36 };
	const int DigitsLabelHeight{ 74 };
};	  // namespace Sizes::Widgets
