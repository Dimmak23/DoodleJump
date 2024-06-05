//* Source header
#include "RandomSpawn.hpp"

//* DL_Framework

//* C++ std
#include <format>
#include <iostream>

//* Custom
//? Modules
#include "Randomizer.hpp"
//? Inheritance
#include "RectangleCore.hpp"
//? Interfaces
#include "I_Universe.hpp"
//? Wrappers
//? Utilities

RandomSpawn::RandomSpawn(IUniverse* parent_world, const unsigned int& sprite_width, const unsigned int& sprite_height)
	: _Parent(parent_world), _SpriteWidth(sprite_width), _SpriteHeight(sprite_height)
{}

RandomSpawn::RandomSpawn(IUniverse* parent_world) : _Parent(parent_world)	 //
{}

RandomSpawn::~RandomSpawn() {}

void RandomSpawn::setOffsetsX(int new_min_offset, int offset_from_min)
{
	_MinOffsetX = new_min_offset;
	_MaxOffsetX = new_min_offset + offset_from_min;
}

void RandomSpawn::setOffsetsY(int new_min_offset, int offset_from_min)
{
	_MinOffsetY = new_min_offset;
	_MaxOffsetY = new_min_offset + offset_from_min;
}

bool RandomSpawn::doWeSpawnByHeight() const { return (Randomizer::getRandomNumber(-_MinOffsetY, _MaxOffsetY) <= 0); }

int RandomSpawn::orderVariantByWidth() const { return Randomizer::getRandomNumber(_MinOffsetX, _MaxOffsetX); }

size_t RandomSpawn::orderNewActors(const Rectangle& new_place_boundary,	   //
								   const unsigned int& variant_qty,		   //
								   UniverseCoorCallBackType callback	   //
) const
{
	if (!_Parent)
	{
		// std::cout << "Spawner don't have any parent Universe pointer...\n";
		return 0;
	}
	if (!callback)
	{
		// std::cout << "orderNewActors function didn't received any callback pointer...\n";
		return 0;
	}

	int quantity{};

	//? From bottom to top
	for (int height{ new_place_boundary.y + new_place_boundary.h }; height > (new_place_boundary.y + _SpriteHeight);)
	{
		int GeneratedOffsetY = Randomizer::getRandomNumber(_MinOffsetY, _MaxOffsetY);

		int GeneratedSkipOptionY = Randomizer::getRandomNumber(-17, 0);
		bool DoWeSkipThisPlaceY{ (GeneratedSkipOptionY >= 0) };

		if (!DoWeSkipThisPlaceY)
		{
			//? From left to right
			for (int width{ new_place_boundary.x };
				 width < (new_place_boundary.x + new_place_boundary.w - _SpriteWidth);)
			{
				int GeneratedOffsetX = Randomizer::getRandomNumber(_MinOffsetX, _MaxOffsetX);

				int GeneratedSkipOptionX = Randomizer::getRandomNumber(-3, 0);
				bool DoWeSkipThisPlaceX{ (GeneratedSkipOptionX >= 0) };

				if (!DoWeSkipThisPlaceX)
				{
					int SmallVarianceX = Randomizer::getRandomNumber(-float(_MinOffsetX) / 2, /* _MinOffsetX / 2 */ 0);
					SmallVarianceX += Randomizer::getRandomNumber(/* -_MinOffsetX / 2 */ 0, _MinOffsetX);
					width += SmallVarianceX;
					if (width < 0)
					{
						width = Randomizer::getRandomNumber(/* -_MinOffsetX / 2 */ 0, float(_MinOffsetX) / 4);
					}
					else if (width > (new_place_boundary.x + new_place_boundary.w - _SpriteWidth))
					{
						width = (new_place_boundary.x + new_place_boundary.w - _SpriteWidth);
						width += Randomizer::getRandomNumber(-float(_MinOffsetX) / 4, 0);
					}

					int SmallVarianceY = Randomizer::getRandomNumber(-float(_MinOffsetY) / 8, 0);
					height += SmallVarianceY;

					quantity++;

					//? Also generating image variant for platform
					unsigned int Variant = Randomizer::getRandomNumber(0, variant_qty - 1);

					//? Finally, create and place spawned Actor
					(_Parent->*callback)(width, height, Variant);

					//? Add another offset to avoid overlapping Actors in a row
					width += _SpriteWidth;
				}

				//? Now typical offset to another column
				width += GeneratedOffsetX;
			}

			//? Add another offset to avoid overlapping Actors in a column
			height -= _SpriteHeight;
		}

		//? Now typical offset to another row
		height -= GeneratedOffsetY;
	}

	// std::cout << std::format("\t\toverall placed {} new Actors.\n", quantity);

	return quantity;
}

void RandomSpawn::orderSingleActor(const int bottom_coordinate_to_spawn, const int screen_width,
								   const unsigned int& variant_qty, UniverseCoorCallBackType callback) const
{
	//? Random number for the image of the first  platform
	unsigned int Variant = Randomizer::getRandomNumber(0, variant_qty - 1);

	//? X coordinate will be random
	int OffsetX = Randomizer::getRandomNumber(0, screen_width - _SpriteWidth - 1);

	//? Finally, create and place spawned Actor
	(_Parent->*callback)(OffsetX, bottom_coordinate_to_spawn, Variant);
}
