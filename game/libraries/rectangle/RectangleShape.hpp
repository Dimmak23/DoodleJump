#pragma once

//* twoDarKade: framework
//? Interfaces
//? Inheritance
#include "RectangleCore.hpp"
//? Utilities

//* C++ std

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

	double* getAngle();	   //! Called from the PhysicsEngine, and possibly from Locator

	//* Setters

	void scale(float symmetrical_scale);
	void scale(float scale_width, float scale_height);

	//* Manipulators

	void resetCenterPoint();

protected:
	//@ API: only for the childrens inner use only

	//^ Members

	//* Non-core elements

	//? Geometry
	double _angle{};					  //? PhysicsEngine accessed this
	Point* _centerRotation{ nullptr };	  //? PhysicsEngine accessed this

	//? Scalling
	const float _InitialScaleWidth{};
	const float _InitialScaleHeight{};

private:
	//? Let's prevent copying of the rectangle
	RectangleShape(const RectangleShape&) = delete;
	RectangleShape& operator=(const RectangleShape&) = delete;
};
