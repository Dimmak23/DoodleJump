#pragma once

//* Game
#include "level/level/I_Stoppable.hpp"
#include "screen/ScreenItem.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class Collider;
class Camera;
class RectangleShape;
class Scene;
class WorldStatic;
class WorldDynamic;
struct Point;
class ParallaxItem;
class Player;
class Gun;
class ScoreBoard;
class JetPack;

class Level : public IStoppable, public ScreenItem
{
public:
	Level(							   //
		const unsigned int& width,	   //
		const unsigned int& height,	   //
		const float& scale_x,		   //
		const float& scale_y		   //
	);
	~Level();

	//@ API: for the game application

	//* Process

	//? Initialize all smart pointers with all level objects (dynamic and static)
	void Initialize(bool& is_running_out);
	//? Tick all objects and change level running state
	void Tick(float delta_t, bool& is_running_out);
	//? Reset all smart pointers to initialize them later
	void Clear();

	//* Setters

	void EnablePlayerGravity(bool enable);

	//* Getters

	bool IsRunning() const;
	bool IsOneLeftMouseButtonClickProcessed() const;

	//* Manipulators

	void OnLeftArrowClicked();
	void OnRightArrowClicked();
	void OnUpArrowClicked();
	void OnDownArrowClicked();
	void OnHorizontalArrowsReleased();
	void OnVerticalArrowsReleased();
	void OnMouseLeftButtonClick(const Point& point);
	void OnMouseLeftButtonReleased();

	//@ API: connection to colider

	virtual void OnLevelStop() override;
	virtual void OnEnemyKilled(long unsigned int index) override;
	virtual void OnAmmoDestroyed(long unsigned int index) override;

private:
	//@ Methods

	//* Let's prevent copying
	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;

	//@ Members

	//* Sprites
	std::unique_ptr<ParallaxItem> _background{ nullptr };
	//* Player
	std::unique_ptr<Player> _player{ nullptr };
	std::unique_ptr<Gun> _gun{ nullptr };
	std::unique_ptr<JetPack> _jet{ nullptr };
	//* Modules
	std::unique_ptr<Collider> _doodieCollider{ nullptr };
	std::unique_ptr<Camera> _doodieCamera{ nullptr };
	//* Frames
	std::unique_ptr<RectangleShape> _gameBoard{ nullptr };
	//* Scene
	std::unique_ptr<Scene> _scene{ nullptr };
	//* Worlds
	std::unique_ptr<WorldStatic> _plaformsWorld{ nullptr };
	std::unique_ptr<WorldDynamic> _enemiesWorld{ nullptr };
	//* Widgets
	std::unique_ptr<ScoreBoard> _score{ nullptr };

	//* Points
	Point* _clickedPosition{ nullptr };

	//* Camera control
	bool _bCameraEnabled{ false };

	//* Level flow
	bool _bIsRunning{ false };
	bool _bLeftMouseButtonClicked{ false };
};
