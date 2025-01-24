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
	: _screen(parent_screen), _tracingCharacter(tracing_body), _imagePath(path)
{
	//? Get serialized viewport width, height and scale them according to the game window
	_viewPort = std::make_unique<Image>(_screen, _imagePath,	//
										viewport_width, viewport_height);
	_viewPort->SetIsHidden(true);

	//? Place camera at correct point
	_viewPortLocator = std::make_unique<Locator>(_viewPort.get());
	_viewPortLocator->SetCenterLocation(_tracingCharacter->Center());

	//? Contructing mover
	_viewPortMover = std::make_unique<PhysicsEngine>(_viewPort.get());
	_viewPortMover->SetConstantVelocity(_cameraSpeedX, _cameraSpeedY);

	_viewPortLocator->SetPhysicsEngineConnection(_viewPortMover.get());

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

// void Camera::SetCharacterIsInFocus(bool new_character_is_in_focus) { _bCharacterIsInFocus =
// new_character_is_in_focus; }

void Camera::SetSceneConnection(ILocatableScene* scene) { _levelScene = scene; }

void Camera::SetCharacterEngine(IMechanics* engine) { _characterEngine = engine; }

void Camera::SetCameraSpeed(float camera_speed_x, float camera_speed_y)
{
	_cameraSpeedX = camera_speed_x;
	_cameraSpeedY = camera_speed_y;
	if (_viewPortMover.get())
	{
		_viewPortMover->SetConstantVelocity(_cameraSpeedX, _cameraSpeedY);
	}
}

void Camera::SetIgnoreSidesMoves(bool ignore_x_side)
{
	_bIgnoreXOffset = ignore_x_side;
	RelocateCamereByIgnore();
}

void Camera::SetIgnoreUpDownMoves(bool ignore_y_side)
{
	_bIgnoreYOffset = ignore_y_side;
	RelocateCamereByIgnore();
}

int Camera::GetViewportWidth() const { return _viewPort->Width(); }

int Camera::GetViewportHeight() const { return _viewPort->Height(); }

int Camera::GetViewportBottom() const { return _viewPort->Bottom(); }

void Camera::Tick(float deltaT)
{
	CheckIfCharacterIsInFocus();
	TraceCharacter(deltaT);
	CheckIfPrecisionIsMet();
}

void Camera::PointCamera(const int& x, const int& y) { _viewPortLocator->SetCenterLocation(x, y); }

void Camera::SetDontGoDown(bool new_state) { _bDontGoDown = new_state; }

void Camera::ClearTrace()
{
	_info->direction = IntersectionDirection::NONE;
	_info->size.width = 0;
	_info->size.height = 0;
}

void Camera::Render() { _viewPort->Render(); }

void Camera::CheckIfPrecisionIsMet()
{
	if (!_bCharacterIsInFocus)
	{
		if (																										 //
			((std::abs(_tracingCharacter->Center().x - _viewPort->Center().x) <= _precisionX) || _bIgnoreXOffset)	 //
			&&																										 //
			((std::abs(_tracingCharacter->Center().y - _viewPort->Center().y) <= _precisionY) || _bIgnoreYOffset)	 //
		)
		{
			_bCharacterIsInFocus = true;
		}
	}
}

void Camera::RelocateCamereByIgnore()
{
	_viewPortLocator->SetCenterLocation(													//
		((_bIgnoreXOffset) ? (_screen->_applicationWidth / 2) : (_viewPort->Center().x)),	//
		((_bIgnoreYOffset) ? (_screen->_applicationHeight / 2) : (_viewPort->Center().y))	//
	);
}

void Camera::OnRelocateScene(int delta_x, int delta_y)
{
	if (_levelScene)
	{
		_levelScene->RelocateAll(delta_x, delta_y);
	}
}

void Camera::CheckIfCharacterIsInFocus()
{
	if (																						//
		((_tracingCharacter->Left() < _viewPort->Left())										//
		 ||																						//
		 (!_bCharacterIsInFocus && (_tracingCharacter->Center().x < _viewPort->Center().x)))	//
		&&																						//
		!_bIgnoreXOffset																		//
	)
	{
		_info->direction = IntersectionDirection::LEFT;
		_info->size.width = _viewPort->Center().x - _tracingCharacter->Center().x;

		_bCharacterIsInFocus = false;
	}
	else if (																					//
		((_tracingCharacter->Right() > _viewPort->Right())										//
		 ||																						//
		 (!_bCharacterIsInFocus && (_tracingCharacter->Center().x > _viewPort->Center().x)))	//
		&&																						//
		!_bIgnoreXOffset																		//
	)
	{
		_info->direction = IntersectionDirection::RIGHT;
		_info->size.width = _tracingCharacter->Center().x - _viewPort->Center().x;

		_bCharacterIsInFocus = false;
	}
	else if (																					//
		((_tracingCharacter->Top() < _viewPort->Top())											//
		 ||																						//
		 (!_bCharacterIsInFocus && (_tracingCharacter->Center().y < _viewPort->Center().y)))	//
		&&																						//
		!_bIgnoreYOffset																		//
	)
	{
		_info->direction = IntersectionDirection::TOP;
		_info->size.height = -(_viewPort->Center().y - _tracingCharacter->Center().y);

		_bCharacterIsInFocus = false;
	}
	else if (																					//
		((_tracingCharacter->Bottom() > _viewPort->Bottom())									//
		 ||																						//
		 (!_bCharacterIsInFocus && (_tracingCharacter->Center().y > _viewPort->Center().y)))	//
		&&																						//
		!_bIgnoreYOffset																		//
		&&																						//
		!_bDontGoDown																			//
	)
	{
		_info->direction = IntersectionDirection::BOTTOM;
		_info->size.height = _tracingCharacter->Center().y - _viewPort->Center().y;

		_bCharacterIsInFocus = false;
	}
	else
	{
		_info->direction = IntersectionDirection::NONE;
		_info->size = { 0, 0 };
	}
}

void Camera::TraceCharacter(float deltaT)
{
	if (!_bCharacterIsInFocus && _info)
	{
		if (_info->direction == IntersectionDirection::NONE)
		{
			return;
		}
		auto CameraDeltas =
			_viewPortMover->ConstantSpeedUp(deltaT, float(_info->size.width), float(_info->size.height));
		// _ViewPortMover->move(); //! turned off, to move scene instead
		OnRelocateScene(-int(CameraDeltas.x), -int(CameraDeltas.y));

		if (!_characterEngine->GetOnPlatform())
		{
			_characterEngine->SetAirFrictionY(0.105);
			// TODO: setted const speed in two independent places, somewhow work correctly
			_characterEngine->SetConstantVelocity(0.f, 0.275f);
		}
		else
		{
			_characterEngine->SetAirFrictionY(0.015);
		}
	}
	else if (_bCharacterIsInFocus)
	{
		auto CameraDeltas = _viewPortMover->ConstantSpeedUp(deltaT, 0, 0);
	}
}
