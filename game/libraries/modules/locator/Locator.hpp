#pragma once

//* Game
#include "modules/locator/I_RelocatableActor.hpp"

//* Forward declarations
class RectangleCore;
struct Rectangle;
struct Point;
class IConnectable;

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
	Locator* ParseBody(Rectangle* body);
	void ResetBody(Rectangle* body);
	void ClearBody();

	//* Setters: callbacks

	void SetPhysicsEngineConnection(IConnectable* physics_engine);
	// void setUpdatePreciseCoordinatesCallBack(ConnectCallBackType function);

	//* Setters: states

	void SetLoggingEnabled(bool new_state);

	//* Setters: location

	//? Two-Dimensional
	void SetCenterLocation(const int& x, const int& y);
	void SetCenterLocation(Point coordinate);
	void SetCenterLocation(Coordinate coordinate);
	void SetTopCLocation(const int& x, const int& y);
	void SetBottomCLocation(const int& x, const int& y);
	void SetLeftCLocation(const int& x, const int& y);
	void SetRightCLocation(const int& x, const int& y);
	void SetLTCornerLocation(const int& x, const int& y);
	void SetLBCornerLocation(const int& x, const int& y);
	void SetRTCornerLocation(const int& x, const int& y);
	void SetRBCornerLocation(const int& x, const int& y);

	//? One-Dimensional
	// void setRight(const  int& x);
	void SetLeft(const int& x);
	void SetTop(const int& y);
	void AlignYAxis(const int& x);

	//* Getters: location

	//  int getTop() const;
	//  int getBottom() const;
	//  int getleft() const;
	//  int getRight() const;
	Coordinate GetTopCLocation() const;
	Coordinate GetCenter() const;
	int GetCenterX() const;
	int GetCenterY() const;
	int GetX() const;
	int GetY() const;

	//* Setters: relocation

	virtual void Relocate(int deltaX, int deltaY) override;

private:
	//@ Methods

	//? Let's prevent copying of the module
	Locator(const Locator&) = delete;
	Locator& operator=(const Locator&) = delete;

	void OnPhisicsEngineUpdate(double x, double y);

	//@ Members

	Rectangle* _host{ nullptr };
	IConnectable* _physicsEngine{ nullptr };

	//* Callbacks
	// ConnectCallBackType _updatePreciseCoordinates{ nullptr };

	//* States
	bool _bLoggingEnabled{ false };
};
