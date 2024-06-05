#pragma once

//* Custom
//? Interfaces
//? Inheritance

//* C++ std

//* Forward declarations

struct Rectangle
{
	int x{};
	int y{};
	int w{};
	int h{};
};

struct Point
{
	Point(int x_, int y_) : x(x_), y(y_) {}
	int x{};
	int y{};
};

struct IntersectionSize
{
	int width{};
	int height{};
};

enum class IntersectionDirection
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	NONE
};

struct IntersectionInfo
{
	IntersectionSize size{};
	IntersectionDirection direction{};
};

class RectangleCore
{
public:
	// TODO: should be constructed with Rectangle
	RectangleCore() = default;
	virtual ~RectangleCore() = default;

	//@ API: can use outside childrens
	//? Getters
	Rectangle* getBody();	 //! Called from the Locator, PhysicsEngine, Collider
	int top() const;
	int bottom() const;
	int left() const;
	int right() const;
	Point center() const;
	unsigned int width() const;
	unsigned int height() const;
	//? Setters
	void setBody(const Rectangle& new_body);

protected:
	//^ Members
	//? Body
	Rectangle _rectangle{ 0, 0, 0, 0 };	   //? PhysicsEngine and Locator accessed this
	// Rectangle _RenderBox{ 0, 0, 0, 0 };	  // TODO: one rectangle for sprite render
	// Rectangle _CollisionBox{ 0, 0, 0, 0 };	  // TODO: another rectangle for collisions

	//* Non-core elements
	//? Geometry
	double _angle{};	//? PhysicsEngine will access this

private:
	//@ Methods

	//* Let's prevent copying of the utility
	RectangleCore(const RectangleCore&) = delete;
	RectangleCore& operator=(const RectangleCore&) = delete;
};
