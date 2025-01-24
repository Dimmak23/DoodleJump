#pragma once

//* Game

//* C++ std
#include <random>

//* Forward declarations

//? Class with static methods for integers generation
class Randomizer
{
public:
	static inline int GetRandomNumber(int left_limit, int right_limit);
};

inline int Randomizer::GetRandomNumber(int left_limit, int right_limit)
{
	if (left_limit == right_limit) return left_limit;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(left_limit,
											right_limit);	 //* get random number from 'left_limit' to 'right_limit'
	return dist(gen);
}
