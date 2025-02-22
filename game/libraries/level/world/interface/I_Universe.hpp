#pragma once

struct UniverseDot
{
	int x{};
	int y{};
};

class IUniverse
{
public:
	// TODO: This is breaking an 'Interface segregation principle' - WorldDynamic doesn't need this
	virtual void OnSpawnNewActor(const int&, const int&, const unsigned int&) = 0;

	/*
	@ get index from [0, N-1] of Static World new generated platform
	@ return UniverseDot with coordinates to place new Enemy
	 */
	virtual UniverseDot TraverseNewPlatforms(long unsigned int) = 0;
};
