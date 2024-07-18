//* Source header
#include "modules/camera/Camera.hpp"

//* SDL2: Connector

//* Game
//? Modules
#include "modules/locator/Locator.hpp"
#include "modules/physics_engine/PhysicsEngine.hpp"
//? Interfaces
#include "modules/physics_engine/I_Mechanics.hpp"
#include "modules/scene/I_LocatableScene.hpp"
//? Wrappers
#include "graphics/image/Image.hpp"
//? Utilities
#include "screen/ScreenItem.hpp"

//* C++ std
#include <cmath>
#include <iostream>

Camera::Camera(const ScreenItem* parent_screen,		   //
			   RectangleCore* tracing_body,			   //
			   const unsigned int& viewport_width,	   //
			   const unsigned int& viewport_height,	   //
			   const char* path)
	: _Screen(parent_screen), _TracingCharacter(tracing_body), _ImagePath(path)
{
	//? Get serialized viewport width, height and scale them according to the game window
	_ViewPort = std::make_unique<Image>(_Screen, _ImagePath,	//
										viewport_width, viewport_height);
	_ViewPort->setIsHidden(true);

	//? Place camera at correct point
	_ViewPortLocator = std::make_unique<Locator>(_ViewPort.get());
	_ViewPortLocator->setCenterLocation(_TracingCharacter->center());

	//? Contructing mover
	_ViewPortMover = std::make_unique<PhysicsEngine>(_ViewPort.get());
	_ViewPortMover->setConstantVelocity(_CameraSpeedX, _CameraSpeedY);

	_ViewPortLocator->setPhysicsEngineConnection(_ViewPortMover.get());

	_info = new IntersectionInfo{};
	_info->direction = IntersectionDirection::NONE;
	_info->size = { 0, 0 };

	// std::cout << "Constructed Camera size of: " << sizeof(*this) << '\n';
}

Camera::~Camera()
{
	delete _info;
	_info = nullptr;
}

// void Camera::setCharacterIsInFocus(bool new_character_is_in_focus) { _bCharacterIsInFocus =
// new_character_is_in_focus; }

void Camera::setSceneConnection(ILocatableScene* scene) { _LevelScene = scene; }

void Camera::setCharacterEngine(IMechanics* engine) { _CharacterEngine = engine; }

void Camera::setCameraSpeed(float camera_speed_x, float camera_speed_y)
{
	_CameraSpeedX = camera_speed_x;
	_CameraSpeedY = camera_speed_y;
	if (_ViewPortMover.get())
	{
		_ViewPortMover->setConstantVelocity(_CameraSpeedX, _CameraSpeedY);
	}
}

void Camera::setIgnoreSidesMoves(bool ignore_x_side)
{
	_bIgnoreXOffset = ignore_x_side;
	relocateCamereByIgnore();
}

void Camera::setIgnoreUpDownMoves(bool ignore_y_side)
{
	_bIgnoreYOffset = ignore_y_side;
	relocateCamereByIgnore();
}

int Camera::getViewportWidth() const { return _ViewPort->width(); }

int Camera::getViewportHeight() const { return _ViewPort->height(); }

int Camera::getViewportBottom() const { return _ViewPort->bottom(); }

void Camera::tick(float deltaT)
{
	checkIfCharacterIsInFocus();
	traceCharacter(deltaT);
	checkIfPrecisionIsMet();
}

void Camera::pointCamera(const int& x, const int& y) { _ViewPortLocator->setCenterLocation(x, y); }

void Camera::setDontGoDown(bool new_state) { _bDontGoDown = new_state; }

void Camera::clearTrace()
{
	_info->direction = IntersectionDirection::NONE;
	_info->size.width = 0;
	_info->size.height = 0;
}

void Camera::render() { _ViewPort->render(); }

void Camera::checkIfPrecisionIsMet()
{
	if (!_bCharacterIsInFocus)
	{
		if (																										 //
			((std::abs(_TracingCharacter->center().x - _ViewPort->center().x) <= _precisionX) || _bIgnoreXOffset)	 //
			&&																										 //
			((std::abs(_TracingCharacter->center().y - _ViewPort->center().y) <= _precisionY) || _bIgnoreYOffset)	 //
		)
		{
			_bCharacterIsInFocus = true;
		}
	}
}

void Camera::relocateCamereByIgnore()
{
	_ViewPortLocator->setCenterLocation(													//
		((_bIgnoreXOffset) ? (_Screen->ApplicationWidth / 2) : (_ViewPort->center().x)),	//
		((_bIgnoreYOffset) ? (_Screen->ApplicationHeight / 2) : (_ViewPort->center().y))	//
	);
}

void Camera::onRelocateScene(int delta_x, int delta_y)
{
	if (_LevelScene)
	{
		_LevelScene->relocateAll(delta_x, delta_y);
	}
}

void Camera::checkIfCharacterIsInFocus()
{
	if (																						//
		((_TracingCharacter->left() < _ViewPort->left())										//
		 ||																						//
		 (!_bCharacterIsInFocus && (_TracingCharacter->center().x < _ViewPort->center().x)))	//
		&&																						//
		!_bIgnoreXOffset																		//
	)
	{
		_info->direction = IntersectionDirection::LEFT;
		_info->size.width = _ViewPort->center().x - _TracingCharacter->center().x;

		_bCharacterIsInFocus = false;
	}
	else if (																					//
		((_TracingCharacter->right() > _ViewPort->right())										//
		 ||																						//
		 (!_bCharacterIsInFocus && (_TracingCharacter->center().x > _ViewPort->center().x)))	//
		&&																						//
		!_bIgnoreXOffset																		//
	)
	{
		_info->direction = IntersectionDirection::RIGHT;
		_info->size.width = _TracingCharacter->center().x - _ViewPort->center().x;

		_bCharacterIsInFocus = false;
	}
	else if (																					//
		((_TracingCharacter->top() < _ViewPort->top())											//
		 ||																						//
		 (!_bCharacterIsInFocus && (_TracingCharacter->center().y < _ViewPort->center().y)))	//
		&&																						//
		!_bIgnoreYOffset																		//
	)
	{
		_info->direction = IntersectionDirection::TOP;
		_info->size.height = -(_ViewPort->center().y - _TracingCharacter->center().y);

		_bCharacterIsInFocus = false;
	}
	else if (																					//
		((_TracingCharacter->bottom() > _ViewPort->bottom())									//
		 ||																						//
		 (!_bCharacterIsInFocus && (_TracingCharacter->center().y > _ViewPort->center().y)))	//
		&&																						//
		!_bIgnoreYOffset																		//
		&&																						//
		!_bDontGoDown																			//
	)
	{
		_info->direction = IntersectionDirection::BOTTOM;
		_info->size.height = _TracingCharacter->center().y - _ViewPort->center().y;

		_bCharacterIsInFocus = false;
	}
	else
	{
		_info->direction = IntersectionDirection::NONE;
		_info->size = { 0, 0 };
	}
}

void Camera::traceCharacter(float deltaT)
{
	if (!_bCharacterIsInFocus && _info)
	{
		if (_info->direction == IntersectionDirection::NONE)
		{
			return;
		}
		auto CameraDeltas = _ViewPortMover->constantSpeedUp(deltaT, _info->size.width, _info->size.height);
		// _ViewPortMover->move(); //! turned off, to move scene instead
		onRelocateScene(-CameraDeltas.x, -CameraDeltas.y);

		if (!_CharacterEngine->getOnPlatform())
		{
			_CharacterEngine->setAirFrictionY(0.105);
			// TODO: setted const speed in two independent places, somewhow work correctly
			_CharacterEngine->setConstantVelocity(0.f, 0.275f);
		}
		else
		{
			_CharacterEngine->setAirFrictionY(0.015);
		}
	}
	else if (_bCharacterIsInFocus)
	{
		auto CameraDeltas = _ViewPortMover->constantSpeedUp(deltaT, 0, 0);
	}
}
