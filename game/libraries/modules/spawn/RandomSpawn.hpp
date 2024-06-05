#pragma once

//* Custom
//? Modules
//? Inheritance
//? Interfaces

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class RectangleCore;	//? inheritance
struct Rectangle;		//? inheritance
class IUniverse;		//? level/world/interface
using UniverseCoorCallBackType = void (IUniverse::*)(const int&, const int&, const unsigned int&);

/*
@ How to use?
@ Construct RandomSpawn pointer for a spicific World, where objects will be spawned
@		and original_boundary, from which sprite width and height could be retrieved;
@ Immidiately after call setOffsetsX(new_min_offset, offset_from_min)
@ 		and setOffsetsY(new_min_offset, offset_from_min) to pass offsetes;
@ Later ewveryframe you can call orderNewActors(new_place_boundary, callback)
@		to spawn new actors;
*/
class RandomSpawn
{
public:
	RandomSpawn(IUniverse* parent_world, const unsigned int& sprite_width, const unsigned int& sprite_height);
	RandomSpawn(IUniverse* parent_world);
	~RandomSpawn();

	//@ API

	//* Getters
	// int exposeRandomNumber(int left_limit, int right_limit);

	//* Setters

	void setOffsetsX(int new_min_offset, int offset_from_min);
	void setOffsetsY(int new_min_offset, int offset_from_min);

	// TODO: maybe rescale API

	//* Manipulators

	//? Set max offset by Y to much greater number then min (only positive numbers)
	//? to get greater probability of 'false' in result, so no spawn will be done
	bool doWeSpawnByHeight() const;

	//? Choose one number from [_MinOffsetX, _MaxOffsetX]
	int orderVariantByWidth() const;

	size_t orderNewActors(const Rectangle& new_place_boundary, const unsigned int& variant_qty,
						  UniverseCoorCallBackType callback) const;

	void orderSingleActor(const int bottom_coordinate_to_spawn, const int screen_width, const unsigned int& variant_qty,
						  UniverseCoorCallBackType callback) const;

private:
	//@ Methods

	//* Let's prevent copying of the module
	RandomSpawn(const RandomSpawn&) = delete;
	RandomSpawn& operator=(const RandomSpawn&) = delete;

	//@ Members

	//* Host geometry
	int _SpriteWidth{};
	int _SpriteHeight{};

	//* Utilities
	int _MinOffsetX{};
	int _MaxOffsetX{};
	int _MinOffsetY{};
	int _MaxOffsetY{};

	IUniverse* _Parent{ nullptr };
};
