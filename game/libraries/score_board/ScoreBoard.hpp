#pragma once

//* Game
#include "modules/locator/I_RelocatableActor.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class RectangleCore;
class Image;
class AnimatedImage;
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
	void SetPlayerBodyConnection(RectangleCore* body);
	//? We gonna use WorldStatic API to fetch count variables (delted platforms and existing bellow platforms)
	void SetStaticWorldConnection(IAccounting* world);

	//* Manipulators
	void Initialize();
	virtual void Relocate(int delta_x, int delta_y) override;
	void Tick(float delta_t);
	void Clear();

	//* Graphics
	void Render();

private:
	//@ Methods

	//* Let's prevent copying
	ScoreBoard(const ScoreBoard&) = delete;
	ScoreBoard& operator=(const ScoreBoard&) = delete;

	//* Reusable widget updater
	void UpdateWidgetsContainer(const size_t& new_counter, size_t& current_counter,
								std::vector<std::unique_ptr<AnimatedImage>>& Digits);

	//@ Members

	//* Widgets
	std::vector<std::unique_ptr<AnimatedImage>> _distanceDigits;
	std::vector<std::unique_ptr<AnimatedImage>> _platformsDigits;
	std::unique_ptr<Image> _distanceLabel{ nullptr };
	std::unique_ptr<Image> _platformsCountLabel{ nullptr };
	std::unique_ptr<Locator> _sharedLocator{ nullptr };

	//* Geometry
	std::unique_ptr<Point> _placement{ nullptr };
	std::unique_ptr<Point> _origin{ nullptr };

	//* Connections
	RectangleCore* _playerBody{ nullptr };
	IAccounting* _platformsWorld{ nullptr };

	//* Graphics
	const ScreenItem* _screen{ nullptr };

	//* Images path
	const char* _distanceLabelPath;
	const char* _platformsLabelPath;
	const char* _digitsLabelsPath;

	//* Scores
	size_t _pixelsCounter{};
	size_t _currentPixelsCounter{};
	size_t _platformsCounter{};
	size_t _currentPlatformsCounter{};

	//* Utilities
	const unsigned int _distanceCountDigits{ 8 };
	const unsigned int _platformsCountDigits{ 6 };
	const unsigned int _digitsQuantity{};
};
