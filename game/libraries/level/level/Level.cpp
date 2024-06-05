//* Source header
#include "Level.hpp"

//* C++ std
#include <format>
#include <iostream>

//* Custom
//? Inheritance
#include "RectangleShape.hpp"
//? Interfaces
//? Modules
#include "Camera.hpp"
#include "Collider.hpp"
#include "Gun.hpp"
#include "Locator.hpp"
#include "ParallaxItem.hpp"
#include "PhysicsEngine.hpp"
#include "Scene.hpp"
//? Abilities
#include "JetPack.hpp"
//? Wrappers
//? Actors
#include "Player.hpp"
//? Worlds
#include "WorldDynamic.hpp"
#include "WorldStatic.hpp"
//? Widgets
#include "ScoreBoard.hpp"
//? Utilities
#include "Path.hpp"
#include "Sizes.hpp"

Level::Level(					   //
	const unsigned int& width,	   //
	const unsigned int& height,	   //
	const float& scale_x,		   //
	const float& scale_y		   //
	)
	: ScreenItem(width, height, scale_x, scale_y)
{}

Level::~Level() {}

void Level::Initialize(bool& is_running_out)
{
	// std::cout << "Initializing level\n";

	_Background =
		std::make_unique<ParallaxItem>(this, Images::Environment::BackgroundSpace, Sizes::Background::DefaultWidth,
									   Sizes::Background::DefaultHeight, MoveVariants::UP_DOWN);

	// TODO: use initializer
	_Player = std::make_unique<Player>(this, Images::Player::SpaceCut);	   //? Created but placement wrong for now
	// _Player->setIsOnTopPlatfrom(true);

	_Jet = std::make_unique<JetPack>();
	_Jet->initialize();
	_Jet->setPhysicsEngineConnection(_Player->getPlayerEngine());

	_DoodieCollider = std::make_unique<Collider>(_Player->getPlayerBoundary());	   //? RectangleCore*
	_DoodieCollider->setParentEngine(_Player->getPlayerEngine());				   //? IMechanics*

	_DoodieCamera = std::make_unique<Camera>(this, _Player->getPlayerBoundary(),	//? RectangleCore*
											 Sizes::Camera::DefaultWidth, Sizes::Camera::DefaultHeight,
											 Images::Camera::DefaultCamera);
	_DoodieCamera->pointCamera(ApplicationWidth / 2, ApplicationHeight / 2);
	_DoodieCamera->setIgnoreSidesMoves(true);
	_DoodieCamera->setCharacterEngine(_Player->getPlayerEngine());
	// _DoodieCamera->setCharacterIsInFocus(false);

	//? Now we can place actors on a scene
	_Scene = std::make_unique<Scene>();

	//? Let's create static world
	_PlaformsWorld =
		std::make_unique<WorldStatic>(this, Images::Environment::PlatformsCut, Images::Environment::PlatformsAdder,
									  Images::Environment::PlatformsQty);
	_PlaformsWorld->setSceneConnection(_Scene.get());
	_PlaformsWorld->setPlayerColliderConnection(_DoodieCollider.get());
	_PlaformsWorld->initialize();
	//? Using callback to collider to hide platforms
	_DoodieCollider->setStaticWorldConnection(_PlaformsWorld.get());

	//? Now we can place Player correctly, cause platforms spawned
	_Player->setInitialPlace(_PlaformsWorld->getFirstPlatformTopCLocation());

	//? Let's create dynamic world
	_EnemiesWorld = std::make_unique<WorldDynamic>(this,								 //
												   Images::Environment::EnemiesCut,		 //
												   Images::Environment::EnemiesAdder,	 //
												   Images::Environment::EnemiesQty,		 //
												   Images::Environment::Wormhole,		 //
												   Images::Projectiles::Ammo);
	_EnemiesWorld->setLevelConnection(this);
	_EnemiesWorld->setSceneConnection(_Scene.get());
	_EnemiesWorld->setPlayerColliderConnection(_DoodieCollider.get());
	_EnemiesWorld->setPlayerBodyConnection(_Player->getPlayerShape());
	_EnemiesWorld->setPlayerEngineConnection(_Player->getPlayerEngine());
	_EnemiesWorld->initialize();

	//? Circle connection
	_PlaformsWorld->setDynamicWorldConnection(_EnemiesWorld.get());
	_EnemiesWorld->setStaticWorldConnection(_PlaformsWorld.get());

	_GameBoard = std::make_unique<RectangleShape>(this);
	_GameBoard->getBody()->w = ApplicationWidth;
	_GameBoard->getBody()->h = ApplicationHeight;

	_DoodieCollider->addBlockedFrame(_GameBoard.get());
	_DoodieCollider->setIsIgnoringBottomFrame(true);
	_DoodieCollider->setIsIgnoringBottomActor(true);
	_DoodieCollider->setIsIgnoringSidesFrame(true);
	_DoodieCollider->setParentLevel(this);

	_Scene->addPlayer(_Player->getPlayerLocator());	   //? IRelocatableActor* get
	_DoodieCamera->setSceneConnection(_Scene.get());

	_Scene->addBackground(_Background.get());

	_Score = std::make_unique<ScoreBoard>(this,								   //
										  Point(ApplicationWidth - 10, 10),	   //
										  Images::Widgets::DistanceLabel,	   //
										  Images::Widgets::PlatformsLabel,	   //
										  Images::Widgets::Digits,			   //
										  Images::Widgets::DigitsQty);
	_Score->initialize();
	_Score->setPlayerBodyConnection(_Player->getPlayerBoundary());
	_Scene->addScoreBoardOrigin(_Score.get());
	_Score->setStaticWorldConnection(_PlaformsWorld.get());

	_bLeftMouseButtonClicked = false;
	_ClickedPosition = new Point(0, 0);

	_Gun = std::make_unique<Gun>();
	_Gun->setOwner(_Player.get());
	_Gun->setDynamicWorldConnection(_EnemiesWorld.get());
	_Gun->setOnSpawnCallBack(&IDynamicSpawn::onSpawnNewMoveableActor);

	_bIsRunning = true;
	is_running_out = _bIsRunning;

	// std::cout << std::format("Initialized Level size of: {}\n", sizeof(*this));
}

void Level::Tick(float delta_t, bool& is_running_out)
{
	//& Moving phase: collisions

	_DoodieCollider->updateCollisions();

	//& Moving phase: positioning

	//? Update player actor features
	_Player->tick(delta_t);

	//? Do we use jet pack right now?
	_Jet->tick(delta_t);

	//? Update score utilities
	_Score->tick(delta_t);

	//? Update platforms, maybe some need to hide
	_PlaformsWorld->tick(delta_t);

	//? Update ammo positions
	_EnemiesWorld->tick(delta_t);

	//? Checking: maybe we need new actors in a static world
	_PlaformsWorld->checkForSpawnNewPlatforms(_Player->getBoundaryTop());
	//? Wormholes places cleared and populated here
	_EnemiesWorld->checkForSpawnNewEnemies(_PlaformsWorld->getNewPlatformsQuantity());
	_EnemiesWorld->checkForSpawnNewWormholes();
	_PlaformsWorld->cleaner();
	_EnemiesWorld->cleaner();

	//? Camera focus update
	if (_Player->getCenterY() <= ApplicationHeight / 2)
	{
		_bCameraEnabled = true;
	}
	else if (_PlaformsWorld->getPlatformsBottom() <= ApplicationHeight)
	{
		_bCameraEnabled = false;
	}

	if (_bCameraEnabled)
	{
		_DoodieCamera->setDontGoDown(false);
		_DoodieCamera->tick(delta_t);
	}
	else
	{
		_DoodieCamera->setDontGoDown(true);
		_DoodieCamera->clearTrace();
	}

	if (_bLeftMouseButtonClicked)
	{
		_Gun->shoot(*_ClickedPosition, delta_t);
	}
	else
	{
		_Player->resetShooting();
		//? There is some time could left on a timer, need to erase it
		_Gun->reload(delta_t);
	}

	//& Rendering phase

	_Background->render();
	_PlaformsWorld->render();
	_EnemiesWorld->render();
	_Player->render();
	_DoodieCamera->render();
	_Score->render();

	//& Maybe we should stop application

	is_running_out = _bIsRunning;
}

void Level::Clear()
{
	_bIsRunning = false;
	_bCameraEnabled = false;

	_Jet->clear();

	_Score->clear();

	//? This should be ok
	_Scene.reset();
	_GameBoard.reset();

	//? Possible problems here
	_DoodieCamera.reset();
	_DoodieCollider.reset();

	//? Depend on Scene and Collider
	_PlaformsWorld.reset();
	_EnemiesWorld.reset();

	//? This should be ok in this order
	_Player->clear();

	//? Last step seems independent
	_Background.reset();

	_bLeftMouseButtonClicked = false;
	delete _ClickedPosition;
	_ClickedPosition = nullptr;
	_Gun.reset();
}

void Level::enablePlayerGravity(bool enable)
{
	if (_Player)
	{
		_Player->enableGravity(enable);
	}
}

bool Level::IsRunning() const { return _bIsRunning; }

void Level::onLeftArrowClicked() { _Player->onWalking(WalkingSide::LEFT); }

void Level::onRightArrowClicked() { _Player->onWalking(WalkingSide::RIGHT); }

void Level::onUpArrowClicked() { _Jet->enable(true); }

void Level::onDownArrowClicked() {}

void Level::onHorizontalArrowsReleased() { _Player->onStopWalking(); }

void Level::onVerticalArrowsReleased() { _Jet->enable(false); }

void Level::onMouseLeftButtonClick(const Point& point)
{
	_bLeftMouseButtonClicked = true;
	*_ClickedPosition = point;
}

void Level::onMouseLeftButtonReleased() { _bLeftMouseButtonClicked = false; }

void Level::onLevelStop() { _bIsRunning = false; }

void Level::onEnemyKilled(size_t index) { _EnemiesWorld->onEnemyKilled(index); }

void Level::onAmmoDestroyed(size_t index) { _EnemiesWorld->onAmmoDestroyed(index); }
