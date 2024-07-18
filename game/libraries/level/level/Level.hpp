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

	void enablePlayerGravity(bool enable);

	//* Getters

	bool IsRunning() const;
	bool IsOneLeftMouseButtonClickProcessed() const;

	//* Manipulators

	void onLeftArrowClicked();
	void onRightArrowClicked();
	void onUpArrowClicked();
	void onDownArrowClicked();
	void onHorizontalArrowsReleased();
	void onVerticalArrowsReleased();
	void onMouseLeftButtonClick(const Point& point);
	void onMouseLeftButtonReleased();

	//@ API: connection to colider

	virtual void onLevelStop() override;
	virtual void onEnemyKilled(size_t index) override;
	virtual void onAmmoDestroyed(size_t index) override;

private:
	//@ Methods

	//* Let's prevent copying
	Level(const Level&) = delete;
	Level& operator=(const Level&) = delete;

	//@ Members

	//* Sprites
	std::unique_ptr<ParallaxItem> _Background{ nullptr };
	//* Player
	std::unique_ptr<Player> _Player{ nullptr };
	std::unique_ptr<Gun> _Gun{ nullptr };
	std::unique_ptr<JetPack> _Jet{ nullptr };
	//* Modules
	std::unique_ptr<Collider> _DoodieCollider{ nullptr };
	std::unique_ptr<Camera> _DoodieCamera{ nullptr };
	//* Frames
	std::unique_ptr<RectangleShape> _GameBoard{ nullptr };
	//* Scene
	std::unique_ptr<Scene> _Scene{ nullptr };
	//* Worlds
	std::unique_ptr<WorldStatic> _PlaformsWorld{ nullptr };
	std::unique_ptr<WorldDynamic> _EnemiesWorld{ nullptr };
	//* Widgets
	std::unique_ptr<ScoreBoard> _Score{ nullptr };

	//* Camera control
	bool _bCameraEnabled{ false };

	//* Level flow
	bool _bIsRunning{ false };
	bool _bLeftMouseButtonClicked{ false };

	//* Points
	Point* _ClickedPosition{ nullptr };
};
