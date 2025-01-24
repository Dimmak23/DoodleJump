#pragma once

//* Game

//* C++ std
#include <memory>

//* Forward declarations
class RectangleCore;
struct Point;
class Image;
class PhysicsEngine;
struct IntersectionInfo;
class Locator;
class ILocatableScene;
struct ScreenItem;
class IMechanics;

/*
@ How to use?
@ Construct Camera pointer with RectangleCore* Character that will be tracked,
@ 	sizes (width, height), and application sizes (width, height).
@ Camera moves with constant speed, and this speed could be reset, use: SetCameraSpeed(float, float).
@ After construction you can set to ignore some sides of camera movement:
@ SetIgnoreSidesMoves(bool) - ignore moves to left/right, SetIgnoreUpDownMoves(bool).
@! IMPORTANT: use tick(float) to update camera position and track character,
@			  render(void): to show camera aim on the screen.
*/

class Camera
{
public:
	Camera(const ScreenItem* parent_screen,		   //
		   RectangleCore* tracing_body,			   //
		   const unsigned int& viewport_width,	   //
		   const unsigned int& viewport_height,	   //
		   const char* path);
	~Camera();

	//@ API

	//* Setters

	// void SetCharacterIsInFocus(bool new_character_is_in_focus);
	void SetSceneConnection(ILocatableScene* scene);

	void SetCharacterEngine(IMechanics* engine);
	void SetCameraSpeed(float camera_speed_x, float camera_speed_y);
	void SetIgnoreSidesMoves(bool ignore_x_side);
	void SetIgnoreUpDownMoves(bool ignore_y_side);

	//* Getters

	int GetViewportWidth() const;
	int GetViewportHeight() const;
	int GetViewportBottom() const;

	//* Manipulators

	void Tick(float deltaT);
	void PointCamera(const int& x, const int& y);
	void SetDontGoDown(bool new_state);
	void ClearTrace();

	//* Render

	void Render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	//& Post-condition: sets _bCharacterIsInFocus to fase, if _TracingCharacter goes outside of the _ViewPort
	void CheckIfCharacterIsInFocus();
	//? if _bCharacterIsInFocus == false
	//? Gets offsets of deltaX and deltaY of _TracingCharacter.center from _ViewPort.center
	//? and tells _ViewPortMover to move _ViewPort
	//& Post-condition: calls Scene::relocateAll(deltaX, deltaY)
	void TraceCharacter(float deltaT);
	//? if _bCharacterIsInFocus == false: check maybe offsets applied close enough and _precisionX, _precisionY met
	//& Post-condition: sets _bCharacterIsInFocus to true, if _ViewPort.center close enough to _TracingCharacter.center
	void CheckIfPrecisionIsMet();

	void RelocateCamereByIgnore();

	void OnRelocateScene(int delta_x, int delta_y);

	//@ Members

	std::unique_ptr<Image> _viewPort{ nullptr };
	std::unique_ptr<Locator> _viewPortLocator{ nullptr };
	std::unique_ptr<PhysicsEngine> _viewPortMover{ nullptr };

	RectangleCore* _tracingCharacter{ nullptr };
	IMechanics* _characterEngine{ nullptr };
	ILocatableScene* _levelScene{ nullptr };

	//* Graphics window
	const ScreenItem* _screen{ nullptr };

	//* Camera viewport image
	const char* _imagePath{ nullptr };

	//* Utilities
	IntersectionInfo* _info{ nullptr };

	//* Mechanics
	float _cameraSpeedX{ 1.f };
	float _cameraSpeedY{ 1.05f };

	float _precisionX{ 1.f };
	float _precisionY{ 1.f };

	//* State
	bool _bIgnoreXOffset{ false };
	bool _bIgnoreYOffset{ false };
	bool _bDontGoDown{ false };
	bool _bCharacterIsInFocus{ true };
};
