#pragma once

//* Custom
//? Modules
//? Inheritance
//? Interfaces

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
@ Camera moves with constant speed, and this speed could be reset, use: setCameraSpeed(float, float).
@ After construction you can set to ignore some sides of camera movement:
@ setIgnoreSidesMoves(bool) - ignore moves to left/right, setIgnoreUpDownMoves(bool).
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

	// void setCharacterIsInFocus(bool new_character_is_in_focus);
	void setSceneConnection(ILocatableScene* scene);

	void setCharacterEngine(IMechanics* engine);
	void setCameraSpeed(float camera_speed_x, float camera_speed_y);
	void setIgnoreSidesMoves(bool ignore_x_side);
	void setIgnoreUpDownMoves(bool ignore_y_side);

	//* Getters

	int getViewportWidth() const;
	int getViewportHeight() const;
	int getViewportBottom() const;

	//* Manipulators

	void tick(float deltaT);
	void pointCamera(const int& x, const int& y);
	void setDontGoDown(bool new_state);
	void clearTrace();

	//* Render

	void render();

private:
	//@ Methods

	//* Let's prevent copying of the module
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	//& Post-condition: sets _bCharacterIsInFocus to fase, if _TracingCharacter goes outside of the _ViewPort
	void checkIfCharacterIsInFocus();
	//? if _bCharacterIsInFocus == false
	//? Gets offsets of deltaX and deltaY of _TracingCharacter.center from _ViewPort.center
	//? and tells _ViewPortMover to move _ViewPort
	//& Post-condition: calls Scene::relocateAll(deltaX, deltaY)
	void traceCharacter(float deltaT);
	//? if _bCharacterIsInFocus == false: check maybe offsets applied close enough and _precisionX, _precisionY met
	//& Post-condition: sets _bCharacterIsInFocus to true, if _ViewPort.center close enough to _TracingCharacter.center
	void checkIfPrecisionIsMet();

	void relocateCamereByIgnore();

	void onRelocateScene(int delta_x, int delta_y);

	//@ Members

	RectangleCore* _TracingCharacter{ nullptr };
	IMechanics* _CharacterEngine{ nullptr };
	std::unique_ptr<Image> _ViewPort{ nullptr };
	std::unique_ptr<Locator> _ViewPortLocator{ nullptr };
	std::unique_ptr<PhysicsEngine> _ViewPortMover{ nullptr };
	ILocatableScene* _LevelScene{ nullptr };

	//* State
	bool _bCharacterIsInFocus{ true };

	//* Mechanics
	float _CameraSpeedX{ 1.f };
	float _CameraSpeedY{ 1.05f };

	//* Utilities
	IntersectionInfo* _info{ nullptr };
	float _precisionX{ 1.f };
	float _precisionY{ 1.f };
	bool _bIgnoreXOffset{ false };
	bool _bIgnoreYOffset{ false };
	bool _bDontGoDown{ false };

	//* Graphics window
	const ScreenItem* _Screen{ nullptr };

	//* Camera viewport image
	const char* _ImagePath{ nullptr };
};
