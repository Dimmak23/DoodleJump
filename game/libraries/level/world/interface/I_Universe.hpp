#pragma once

struct UniverseDot
{
	int x{};
	int y{};
};

class IUniverse
{
public:
	virtual void onSpawnNewActor(const int&, const int&, const unsigned int&) = 0;

	/*
	@ get index from [0, N-1] of Static World new generated platform
	@ return UniverseDot with coordinates to place new Enemy
	 */
	virtual UniverseDot traverseNewPlatforms(unsigned long long) = 0;
};
