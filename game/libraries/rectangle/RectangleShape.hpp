#pragma once

//* Game
#include "rectangle/RectangleCore.hpp"

//* Forward declarations
struct ScreenItem;

class RectangleShape : public RectangleCore
{
public:
	RectangleShape();
	RectangleShape(const ScreenItem* parent_screen);
	virtual ~RectangleShape();

	//@ API: can use outside childrens

	//* Getters

	double* GetAngle();	   //! Called from the PhysicsEngine, and possibly from Locator

	//* Setters

	void Scale(float symmetrical_scale);
	void Scale(float scale_width, float scale_height);

	//* Manipulators

	void ResetCenterPoint();

protected:
	//@ API: only for the childrens inner use only

	//^ Members

	//* Non-core elements

	//? Geometry
	Point* _centerRotation{ nullptr };	  //? PhysicsEngine accessed this
	double _angle{};					  //? PhysicsEngine accessed this

	//? Scalling
	const float _initialScaleWidth{};
	const float _initialScaleHeight{};

private:
	//? Let's prevent copying of the rectangle
	RectangleShape(const RectangleShape&) = delete;
	RectangleShape& operator=(const RectangleShape&) = delete;
};
