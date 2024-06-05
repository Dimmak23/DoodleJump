#pragma once

//* Custom
//? Inheritance
//? Interfaces
//? Modules

//* C++ std
#include <memory>

//* Forward declarations
class RectangleShape;
class RectangleCore;
class Locator;
struct ScreenItem;
class IMechanics;
class PhysicsEngine;

class Wormhole
{
public:
	Wormhole(								//
		const ScreenItem* parent_screen,	//
		IMechanics* character_engine,		//
		RectangleShape* character_body,		//
		RectangleCore* aim_body				//
	);
	~Wormhole();

	//@ API for dynamic world

	//* Getters
	bool IsStucked() const;

	//* Manipulators
	void tick(float delta_t);

private:
	//@ Methods

	//* Let's prevent copying of the module
	Wormhole(const Wormhole&) = delete;
	Wormhole& operator=(const Wormhole&) = delete;

	//@ Members

	//* Need to know state of the character engine
	IMechanics* _CharacterEngine{ nullptr };	//!
	//* Character body: to create physics engine with it
	RectangleShape* _PossesingBody{ nullptr };
	//* Wormhole own physics engine for character
	//? We don't want to erase some fields from character own PE
	std::unique_ptr<PhysicsEngine> _PossesingMover{ nullptr };
	//* Destination point will be retreived from wormhole entity
	//? It's coordinates can be changed because of camera movements
	RectangleCore* _Aim{ nullptr };
	//* Reusable locator
	std::unique_ptr<Locator> _SharedLocator{ nullptr };

	//* Design full speed
	double _MaximumLinearSpeed{ 1.95 };

	//* When we activate?
	const int _ActivationDistance{};

	//* States
	bool _bIsStacked{ false };
};
