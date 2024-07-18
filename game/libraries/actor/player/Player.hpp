#pragma once

//* Game
#include "actor/interface/I_Shooter.hpp"

//* C++ std
#include <memory>

//* Forward declarations
struct ScreenItem;
class AnimatedImage;
struct Point;
class Locator;
class PhysicsEngine;
class RectangleCore;
class RectangleShape;
class IRelocatableActor;
class IMechanics;

enum class WalkingSide : int
{
	LEFT,
	RIGHT,
	NONE
};

class Player : public IShooter
{
public:
	Player(const ScreenItem* parent_screen, const char* path);
	~Player();

	//@ API for level

	//* Getters

	RectangleCore* getPlayerBoundary();
	RectangleShape* getPlayerShape();
	IRelocatableActor* getPlayerLocator();
	// PhysicsEngine* getPlayerFullEngine();
	IMechanics* getPlayerEngine();
	virtual const Point* getSpawnPoint() override;
	int getBoundaryTop();
	int getCenterY();

	//* Setters

	virtual void setShootingSide(const Point& aim) override;
	void resetShooting();
	void setIsOnTopPlatfrom(bool new_state);

	//* Manipulators

	void initialize();
	void setInitialPlace(Point coordinate);
	void enableGravity(bool enable);
	void tick(float delta_t);
	void clear();

	//* Keyboard implementations

	void onWalking(WalkingSide side);
	void onUpArrowClicked();	  //! TESTING FEATURE
	void onDownArrowClicked();	  //! TESTING FEATURE
	void onStopWalking();
	void onVerticalArrowsReleased();	//! TESTING FEATURE

	//* Graphics

	void render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	void changeShootingSide(WalkingSide side);

	const Point* updateSpawnPoint();

	//@ Members

	//* Core
	std::unique_ptr<AnimatedImage> _DoodieAnimation{ nullptr };
	std::unique_ptr<Point> _DoodieSpawnPoint{ nullptr };
	std::unique_ptr<Locator> _DoodieLocator{ nullptr };
	std::unique_ptr<PhysicsEngine> _DoodieMover{ nullptr };

	//* Graphics window
	const ScreenItem* _Screen{ nullptr };	 //! Maybe unused

	//* States
	bool _bIsShooting{ false };
	WalkingSide _CurrentLookSide{ WalkingSide::NONE };
	bool _bForceAdded{ false };

	//* Geometry
	const unsigned int _SpawnPointHeight{};

	//* Mechanics
	const float _Mass{ 200.f };
	unsigned int _CurrentFlightDistance{};
	const unsigned int _FlightDestination{};
	float _JumpImpulse{ -80.f };
	float _SideTilt{ 75.f };

	//* Default image path
	const char* _ImagePath{ nullptr };
};
