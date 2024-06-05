#pragma once

//* Custom
//? Modules
//? Inheritance
//? Interfaces
#include "I_RelocatableActor.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class RectangleCore;
class SpriteWrapper;
class AnimatedSpriteWrapper;
struct Point;
class IAccounting;
struct ScreenItem;
class Locator;

/*
@ Construct pointer to ScoreBoard object with coordinate {x, y}
@ of {right, top} corner where score board will be placed.
@ call initialize() to load all sprites.
@ call setPlayerBodyConnection(RectangleCore* body) and connect score board with player
@ connect ScoreBoard* to Scene*: scene will update _Origin location.
@ call setStaticWorldConnection(IAccounting* world) to connect platforms world and call accounting features
@ call tick(float delta_t) to update game score.
@ call clear() when level was reload.
*/

class ScoreBoard : public IRelocatableActor
{
public:
	ScoreBoard(const ScreenItem* parent_screen,			   //
			   const Point& right_top_corner_placement,	   //
			   const char* distance_label_path,			   //
			   const char* platforms_label_path,		   //
			   const char* digits_label_path,			   //
			   const unsigned int digits_qty);
	~ScoreBoard();

	//@ API: for level

	//* Setters
	void setPlayerBodyConnection(RectangleCore* body);
	//? We gonna use WorldStatic API to fetch count variables (delted platforms and existing bellow platforms)
	void setStaticWorldConnection(IAccounting* world);

	//* Manipulators
	void initialize();
	virtual void relocate(int delta_x, int delta_y) override;
	void tick(float delta_t);
	void clear();

	//* Graphics
	void render();

private:
	//@ Methods

	//* Let's prevent copying
	ScoreBoard(const ScoreBoard&) = delete;
	ScoreBoard& operator=(const ScoreBoard&) = delete;

	//* Reusable widget updater
	void updateWidgetsContainer(const size_t& new_counter, size_t& current_counter,
								std::vector<std::unique_ptr<AnimatedSpriteWrapper>>& Digits);

	//@ Members

	//* Connections
	RectangleCore* _PlayerBody{ nullptr };
	IAccounting* _PlatformsWorld{ nullptr };

	//* Widgets
	std::unique_ptr<SpriteWrapper> _DistanceLabel{ nullptr };
	std::unique_ptr<SpriteWrapper> _PlatformsCountLabel{ nullptr };
	std::vector<std::unique_ptr<AnimatedSpriteWrapper>> _DistanceDigits;
	std::vector<std::unique_ptr<AnimatedSpriteWrapper>> _PlatformsDigits;
	std::unique_ptr<Locator> _SharedLocator{ nullptr };

	//* Geometry
	std::unique_ptr<Point> _Placement{ nullptr };
	std::unique_ptr<Point> _Origin{ nullptr };

	//* Graphics
	const ScreenItem* _Screen{ nullptr };

	//* Scores
	size_t _PixelsCounter{};
	size_t _CurrentPixelsCounter{};
	size_t _PlatformsCounter{};
	size_t _CurrentPlatformsCounter{};

	//* Utilities
	const unsigned int _DistanceCountDigits{ 8 };
	const unsigned int _PlatformsCountDigits{ 6 };

	//* Images path
	const char* _DistanceLabelPath;
	const char* _PlatformsLabelPath;
	const char* _DigitsLabelsPath;
	const unsigned int _DigitsQuantity{};
};
