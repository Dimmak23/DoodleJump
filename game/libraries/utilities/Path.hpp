#pragma once

namespace Images::Player
{
	const char* SpaceRight{ "data/images/space-right@2x.png" };
	const char* SpaceCut{ "data/images/player-space-" };
};	  // namespace Images::Player

namespace Images::Camera
{
	const char* DefaultCamera{ "data/images/CameraAim.png" };
};	  // namespace Images::Camera

namespace Images::Environment
{
	const char* BackgroundSpace{ "data/images/space-bck@2x.png" };
	const char* PlatformsCut{ "data/images/platform-0" };
	const char* PlatformsAdder{ "-space@2x.png" };
	const unsigned int PlatformsQty{ 5 };
	const char* EnemiesCut{ "data/images/enemy-0" };
	const char* EnemiesAdder{ "-space@2x.png" };
	const unsigned int EnemiesQty{ 5 };
	const char* Wormhole{ "data/images/wormhole.png" };
};	  // namespace Images::Environment

namespace Images::Projectiles
{
	const char* Ammo{ "data/images/bubble@2x.png" };
};	  // namespace Images::Projectiles

namespace Images::Widgets
{
	const char* DistanceLabel{ "data/images/DistanceLabel.png" };
	const char* PlatformsLabel{ "data/images/PlatformsLabel.png" };
	const unsigned int DigitsQty{ 10 };
	const char* Digits{ "data/images/Digit_" };
};	  // namespace Images::Widgets
