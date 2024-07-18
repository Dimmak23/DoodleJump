#pragma once

//* Game
#include "modules/locator/I_RelocatableActor.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
struct ScreenItem;
class Image;
class Locator;

enum class MovingSide : int
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	QTY
};

enum class MoveVariants : int
{
	UP_DOWN,
	LEFT_RIGHT,
	BOTH
};

class ParallaxItem : public IRelocatableActor
{
public:
	ParallaxItem(							   //
		const ScreenItem* parent_screen,	   //
		const char* path,					   //
		const unsigned int& desired_width,	   //
		const unsigned int& desired_height,	   //
		const MoveVariants& variant			   //
	);
	~ParallaxItem();

	//@ API for level

	//* Setters

	//* Getters

	//* Manipulators

	virtual void relocate(int delta_x, int delta_y) override;

	//* Graphics

	void render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	ParallaxItem(const ParallaxItem&) = delete;
	ParallaxItem& operator=(const ParallaxItem&) = delete;

	//@ Members

	//* Modules
	std::vector<std::unique_ptr<Image>> _Images;
	std::unique_ptr<Locator> _Locator{ nullptr };

	//* Parent window
	const ScreenItem* _Screen{ nullptr };

	bool _bNotMovedYet{ true };
};
