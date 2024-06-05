//@ twoDarKade framework: property of Dmytro Kovryzhenko
#pragma once

//* Custom
//? Interfaces
#include "I_RelocatableActor.hpp"
//? Inheritance
// #include "RectangleCore.hpp"

//* C++ std

//* Forward declarations
class RectangleCore;
struct Rectangle;
struct Point;
class IConnectable;
// using ConnectCallBackType = void (IConnectable::*)(double, double);

class Locator : public IRelocatableActor
{
public:
	Locator(RectangleCore* rectangle);
	~Locator();

	//@ API

	struct Coordinate
	{
		Coordinate(int _x, int _y) : x(_x), y(_y) {}
		int x;
		int y;
	};

	//* Body
	Locator* parseBody(Rectangle* body);
	void resetBody(Rectangle* body);
	void clearBody();

	//* Setters: callbacks

	void setPhysicsEngineConnection(IConnectable* physics_engine);
	// void setUpdatePreciseCoordinatesCallBack(ConnectCallBackType function);

	//* Setters: states

	void setLoggingEnabled(bool new_state);

	//* Setters: location

	//? Two-Dimensional
	void setCenterLocation(const int& x, const int& y);
	void setCenterLocation(Point coordinate);
	void setCenterLocation(Coordinate coordinate);
	void setTopCLocation(const int& x, const int& y);
	void setBottomCLocation(const int& x, const int& y);
	void setLeftCLocation(const int& x, const int& y);
	void setRightCLocation(const int& x, const int& y);
	void setLTCornerLocation(const int& x, const int& y);
	void setLBCornerLocation(const int& x, const int& y);
	void setRTCornerLocation(const int& x, const int& y);
	void setRBCornerLocation(const int& x, const int& y);

	//? One-Dimensional
	// void setRight(const  int& x);
	void setLeft(const int& x);
	void setTop(const int& y);
	void alignYAxis(const int& x);

	//* Getters: location

	//  int getTop() const;
	//  int getBottom() const;
	//  int getleft() const;
	//  int getRight() const;
	Coordinate getTopCLocation() const;
	Coordinate getCenter() const;
	int getCenterX() const;
	int getCenterY() const;
	int getX() const;
	int getY() const;

	//* Setters: relocation

	virtual void relocate(int deltaX, int deltaY) override;

private:
	//@ Methods

	//? Let's prevent copying of the module
	Locator(const Locator&) = delete;
	Locator& operator=(const Locator&) = delete;

	void onPhisicsEngineUpdate(double x, double y);

	//@ Members

	Rectangle* _host{ nullptr };

	IConnectable* _PhysicsEngine{ nullptr };

	//* Callbacks
	// ConnectCallBackType _UpdatePreciseCoordinates{ nullptr };

	//* States
	bool _bLoggingEnabled{ false };
};
