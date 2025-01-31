//* Source header
#include "modules/spawn/RandomSpawn.hpp"

//* Game
#include "level/world/interface/I_Universe.hpp"
#include "modules/spawn/Randomizer.hpp"
#include "rectangle/RectangleCore.hpp"

//* C++ std
#include <format>
#include <iostream>

RandomSpawn::RandomSpawn(IUniverse* parent_world, const unsigned int& sprite_width, const unsigned int& sprite_height)
	: _parent(parent_world), _spriteWidth(sprite_width), _spriteHeight(sprite_height)
{}

RandomSpawn::RandomSpawn(IUniverse* parent_world) : _parent(parent_world)	 //
{}

RandomSpawn::~RandomSpawn() {}

void RandomSpawn::SetOffsetsX(int new_min_offset, int offset_from_min)
{
	_minOffsetX = new_min_offset;
	_maxOffsetX = new_min_offset + offset_from_min;
}

void RandomSpawn::SetOffsetsY(int new_min_offset, int offset_from_min)
{
	_minOffsetY = new_min_offset;
	_maxOffsetY = new_min_offset + offset_from_min;
}

bool RandomSpawn::DoWeSpawnByHeight() const { return (Randomizer::GetRandomNumber(-_minOffsetY, _maxOffsetY) <= 0); }

int RandomSpawn::OrderVariantByWidth() const { return Randomizer::GetRandomNumber(_minOffsetX, _maxOffsetX); }

long unsigned int RandomSpawn::OrderNewActors(const Rectangle& new_place_boundary,	  //
											  const unsigned int& variant_qty,		  //
											  UniverseCoorCallBackType callback		  //
) const
{
	if (!_parent)
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
	for (int height{ new_place_boundary.y + new_place_boundary.h }; height > (new_place_boundary.y + _spriteHeight);)
	{
		int GeneratedOffsetY = Randomizer::GetRandomNumber(_minOffsetY, _maxOffsetY);

		int GeneratedSkipOptionY = Randomizer::GetRandomNumber(-17, 0);
		bool DoWeSkipThisPlaceY{ (GeneratedSkipOptionY >= 0) };

		if (!DoWeSkipThisPlaceY)
		{
			//? From left to right
			for (int width{ new_place_boundary.x };
				 width < (new_place_boundary.x + new_place_boundary.w - _spriteWidth);)
			{
				int GeneratedOffsetX = Randomizer::GetRandomNumber(_minOffsetX, _maxOffsetX);

				int GeneratedSkipOptionX = Randomizer::GetRandomNumber(-3, 0);
				bool DoWeSkipThisPlaceX{ (GeneratedSkipOptionX >= 0) };

				if (!DoWeSkipThisPlaceX)
				{
					int SmallVarianceX =
						Randomizer::GetRandomNumber(int(-float(_minOffsetX) / 2), /* _MinOffsetX / 2 */ 0);
					SmallVarianceX += Randomizer::GetRandomNumber(/* -_MinOffsetX / 2 */ 0, _minOffsetX);
					width += SmallVarianceX;
					if (width < 0)
					{
						width = Randomizer::GetRandomNumber(/* -_MinOffsetX / 2 */ 0, int(float(_minOffsetX) / 4));
					}
					else if (width > (new_place_boundary.x + new_place_boundary.w - _spriteWidth))
					{
						width = (new_place_boundary.x + new_place_boundary.w - _spriteWidth);
						width += Randomizer::GetRandomNumber(int(-float(_minOffsetX) / 4), 0);
					}

					int SmallVarianceY = Randomizer::GetRandomNumber(int(-float(_minOffsetY) / 8), 0);
					height += SmallVarianceY;

					quantity++;

					//? Also generating image variant for platform
					unsigned int Variant = Randomizer::GetRandomNumber(0, variant_qty - 1);

					//? Finally, create and place spawned Actor
					(_parent->*callback)(width, height, Variant);

					//? Add another offset to avoid overlapping Actors in a row
					width += _spriteWidth;
				}

				//? Now typical offset to another column
				width += GeneratedOffsetX;
			}

			//? Add another offset to avoid overlapping Actors in a column
			height -= _spriteHeight;
		}

		//? Now typical offset to another row
		height -= GeneratedOffsetY;
	}

	// std::cout << std::format("\t\toverall placed {} new Actors.\n", quantity);

	return quantity;
}

void RandomSpawn::OrderSingleActor(const int bottom_coordinate_to_spawn, const int screen_width,
								   const unsigned int& variant_qty, UniverseCoorCallBackType callback) const
{
	//? Random number for the image of the first  platform
	unsigned int Variant = Randomizer::GetRandomNumber(0, variant_qty - 1);

	//? X coordinate will be random
	int OffsetX = Randomizer::GetRandomNumber(0, screen_width - _spriteWidth - 1);

	//? Finally, create and place spawned Actor
	(_parent->*callback)(OffsetX, bottom_coordinate_to_spawn, Variant);
}
