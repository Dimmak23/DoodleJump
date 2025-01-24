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

	RectangleCore* GetPlayerBoundary();
	RectangleShape* GetPlayerShape();
	IRelocatableActor* GetPlayerLocator();
	// PhysicsEngine* GetPlayerFullEngine();
	IMechanics* GetPlayerEngine();
	virtual const Point* GetSpawnPoint() override;
	int GetBoundaryTop();
	int GetCenterY();

	//* Setters

	virtual void SetShootingSide(const Point& aim) override;
	void ResetShooting();
	void SetIsOnTopPlatfrom(bool new_state);

	//* Manipulators

	void Initialize();
	void SetInitialPlace(Point coordinate);
	void EnableGravity(bool enable);
	void Tick(float delta_t);
	void Clear();

	//* Keyboard implementations

	void OnWalking(WalkingSide side);
	void OnUpArrowClicked();	  //! TESTING FEATURE
	void OnDownArrowClicked();	  //! TESTING FEATURE
	void OnStopWalking();
	void OnVerticalArrowsReleased();	//! TESTING FEATURE

	//* Graphics

	void Render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;

	void ChangeShootingSide(WalkingSide side);

	const Point* UpdateSpawnPoint();

	//@ Members

	//* Core
	std::unique_ptr<AnimatedImage> _doodieAnimation{ nullptr };
	std::unique_ptr<Point> _doodieSpawnPoint{ nullptr };
	std::unique_ptr<Locator> _doodieLocator{ nullptr };
	std::unique_ptr<PhysicsEngine> _doodieMover{ nullptr };

	//* Graphics window
	const ScreenItem* _screen{ nullptr };

	//* Default image path
	const char* _imagePath{ nullptr };

	//* Geometry
	const unsigned int _spawnPointHeight{};

	//* Mechanics
	unsigned int _currentFlightDistance{};
	const unsigned int _flightDestination{};
	const float _mass{ 200.f };
	float _jumpImpulse{ -80.f };
	float _sideTilt{ 75.f };

	//* States
	WalkingSide _currentLookSide{ WalkingSide::NONE };
	bool _bIsShooting{ false };
	bool _bForceAdded{ false };
};
