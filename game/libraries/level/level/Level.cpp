//* Source header
#include "level/level/Level.hpp"

//* Game
#include "abilities/jet_pack/JetPack.hpp"
#include "actor/player/Player.hpp"
#include "level/world/dynamic/WorldDynamic.hpp"
#include "level/world/static/WorldStatic.hpp"
#include "modules/camera/Camera.hpp"
#include "modules/collider/Collider.hpp"
#include "modules/gun/Gun.hpp"
#include "modules/locator/Locator.hpp"
#include "modules/parallax/ParallaxItem.hpp"
#include "modules/physics_engine/PhysicsEngine.hpp"
#include "modules/scene/Scene.hpp"
#include "rectangle/RectangleShape.hpp"
#include "score_board/ScoreBoard.hpp"
#include "utilities/Path.hpp"
#include "utilities/Sizes.hpp"

//* C++ std
#include <format>
#include <iostream>

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

	_background =
		std::make_unique<ParallaxItem>(this, Images::Environment::BackgroundSpace, Sizes::Background::DefaultWidth,
									   Sizes::Background::DefaultHeight, MoveVariants::UP_DOWN);

	// TODO: use initializer
	_player = std::make_unique<Player>(this, Images::Player::SpaceCut);	   //? Created but placement wrong for now
	// _Player->setIsOnTopPlatfrom(true);

	_jet = std::make_unique<JetPack>();
	_jet->Init();
	_jet->SetPhysicsEngineConnection(_player->GetPlayerEngine());

	_doodieCollider = std::make_unique<Collider>(_player->GetPlayerBoundary());	   //? RectangleCore*
	_doodieCollider->SetParentEngine(_player->GetPlayerEngine());				   //? IMechanics*

	_doodieCamera = std::make_unique<Camera>(this, _player->GetPlayerBoundary(),	//? RectangleCore*
											 Sizes::Camera::DefaultWidth, Sizes::Camera::DefaultHeight,
											 Images::Camera::DefaultCamera);
	_doodieCamera->PointCamera(_applicationWidth / 2, _applicationHeight / 2);
	_doodieCamera->SetIgnoreSidesMoves(true);
	_doodieCamera->SetCharacterEngine(_player->GetPlayerEngine());
	// _DoodieCamera->SetCharacterIsInFocus(false);

	//? Now we can place actors on a scene
	_scene = std::make_unique<Scene>();

	//? Let's create static world
	_plaformsWorld =
		std::make_unique<WorldStatic>(this, Images::Environment::PlatformsCut, Images::Environment::PlatformsAdder,
									  Images::Environment::PlatformsQty);
	_plaformsWorld->SetSceneConnection(_scene.get());
	_plaformsWorld->SetPlayerColliderConnection(_doodieCollider.get());
	_plaformsWorld->Initialize();
	//? Using callback to collider to hide platforms
	_doodieCollider->SetStaticWorldConnection(_plaformsWorld.get());

	//? Now we can place Player correctly, cause platforms spawned
	_player->SetInitialPlace(_plaformsWorld->GetFirstPlatformTopCLocation());

	//? Let's create dynamic world
	_enemiesWorld = std::make_unique<WorldDynamic>(this,								 //
												   Images::Environment::EnemiesCut,		 //
												   Images::Environment::EnemiesAdder,	 //
												   Images::Environment::EnemiesQty,		 //
												   Images::Environment::Wormhole,		 //
												   Images::Projectiles::Ammo);
	_enemiesWorld->SetLevelConnection(this);
	_enemiesWorld->SetSceneConnection(_scene.get());
	_enemiesWorld->SetPlayerColliderConnection(_doodieCollider.get());
	_enemiesWorld->SetPlayerBodyConnection(_player->GetPlayerShape());
	_enemiesWorld->SetPlayerEngineConnection(_player->GetPlayerEngine());
	_enemiesWorld->Initialize();

	//? Circle connection
	_plaformsWorld->SetDynamicWorldConnection(_enemiesWorld.get());
	_enemiesWorld->SetStaticWorldConnection(_plaformsWorld.get());

	_gameBoard = std::make_unique<RectangleShape>(this);
	_gameBoard->GetBody()->w = _applicationWidth;
	_gameBoard->GetBody()->h = _applicationHeight;

	_doodieCollider->AddBlockedFrame(_gameBoard.get());
	_doodieCollider->SetIsIgnoringBottomFrame(true);
	_doodieCollider->SetIsIgnoringBottomActor(true);
	_doodieCollider->SetIsIgnoringSidesFrame(true);
	_doodieCollider->SetParentLevel(this);

	_scene->AddPlayer(_player->GetPlayerLocator());	   //? IRelocatableActor* get
	_doodieCamera->SetSceneConnection(_scene.get());

	_scene->AddBackground(_background.get());

	_score = std::make_unique<ScoreBoard>(this,									//
										  Point(_applicationWidth - 10, 10),	//
										  Images::Widgets::DistanceLabel,		//
										  Images::Widgets::PlatformsLabel,		//
										  Images::Widgets::Digits,				//
										  Images::Widgets::DigitsQty);
	_score->Initialize();
	_score->SetPlayerBodyConnection(_player->GetPlayerBoundary());
	_scene->AddScoreBoardOrigin(_score.get());
	_score->SetStaticWorldConnection(_plaformsWorld.get());

	_bLeftMouseButtonClicked = false;
	_clickedPosition = new Point(0, 0);

	_gun = std::make_unique<Gun>();
	_gun->SetOwner(_player.get());
	_gun->SetDynamicWorldConnection(_enemiesWorld.get());
	// ! This is working for the GNU generators, but compiling with VS assign this to 0xFFF...F, why?
	_gun->SetOnSpawnCallBack(&IDynamicSpawn::OnSpawnNewMoveableActor);

	_bIsRunning = true;
	is_running_out = _bIsRunning;
}

void Level::Tick(float delta_t, bool& is_running_out)
{
	//& Moving phase: collisions

	_doodieCollider->UpdateCollisions();

	//& Moving phase: positioning

	//? Update player actor features
	_player->Tick(delta_t);

	//? Do we use jet pack right now?
	_jet->Tick(delta_t);

	//? Update score utilities
	_score->Tick(delta_t);

	//? Update platforms, maybe some need to hide
	_plaformsWorld->Tick(delta_t);

	//? Update ammo positions
	_enemiesWorld->Tick(delta_t);

	//? Checking: maybe we need new actors in a static world
	_plaformsWorld->CheckForSpawnNewPlatforms(_player->GetBoundaryTop());
	//? Wormholes places cleared and populated here
	_enemiesWorld->CheckForSpawnNewEnemies(_plaformsWorld->GetNewPlatformsQuantity());
	_enemiesWorld->CheckForSpawnNewWormholes();
	_plaformsWorld->Cleaner();
	_enemiesWorld->Cleaner();

	//? Camera focus update
	if (_player->GetCenterY() <= int(_applicationHeight / 2))
	{
		_bCameraEnabled = true;
	}
	else if (_plaformsWorld->GetPlatformsBottom() <= int(_applicationHeight))
	{
		_bCameraEnabled = false;
	}

	if (_bCameraEnabled)
	{
		_doodieCamera->SetDontGoDown(false);
		_doodieCamera->Tick(delta_t);
	}
	else
	{
		_doodieCamera->SetDontGoDown(true);
		_doodieCamera->ClearTrace();
	}

	if (_bLeftMouseButtonClicked)
	{
		_gun->Shoot(*_clickedPosition, delta_t);
	}
	else
	{
		_player->ResetShooting();
		//? There is some time could left on a timer, need to erase it
		_gun->Reload(delta_t);
	}

	//& Rendering phase

	_background->Render();
	_plaformsWorld->Render();
	_enemiesWorld->Render();
	_player->Render();
	_doodieCamera->Render();
	_score->Render();

	//& Maybe we should stop application

	is_running_out = _bIsRunning;
}

void Level::Clear()
{
	_bIsRunning = false;
	_bCameraEnabled = false;

	_jet->Clear();

	_score->Clear();

	//? This should be ok
	_scene.reset();
	_gameBoard.reset();

	//? Possible problems here
	_doodieCamera.reset();
	_doodieCollider.reset();

	//? Depend on Scene and Collider
	_plaformsWorld.reset();
	_enemiesWorld.reset();

	//? This should be ok in this order
	_player->Clear();

	//? Last step seems independent
	_background.reset();

	_bLeftMouseButtonClicked = false;
	delete _clickedPosition;
	_clickedPosition = nullptr;
	_gun.reset();
}

void Level::EnablePlayerGravity(bool enable)
{
	if (_player)
	{
		_player->EnableGravity(enable);
	}
}

bool Level::IsRunning() const { return _bIsRunning; }

void Level::OnLeftArrowClicked() { _player->OnWalking(WalkingSide::LEFT); }

void Level::OnRightArrowClicked() { _player->OnWalking(WalkingSide::RIGHT); }

void Level::OnUpArrowClicked() { _jet->Enable(true); }

void Level::OnDownArrowClicked() {}

void Level::OnHorizontalArrowsReleased() { _player->OnStopWalking(); }

void Level::OnVerticalArrowsReleased() { _jet->Enable(false); }

void Level::OnMouseLeftButtonClick(const Point& point)
{
	_bLeftMouseButtonClicked = true;
	*_clickedPosition = point;
}

void Level::OnMouseLeftButtonReleased() { _bLeftMouseButtonClicked = false; }

void Level::OnLevelStop() { _bIsRunning = false; }

void Level::OnEnemyKilled(long unsigned int index) { _enemiesWorld->OnEnemyKilled(index); }

void Level::OnAmmoDestroyed(long unsigned int index) { _enemiesWorld->OnAmmoDestroyed(index); }
