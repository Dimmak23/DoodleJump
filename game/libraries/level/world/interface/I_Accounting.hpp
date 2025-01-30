#pragma once

//* Forward declarations
// using size_t = long unsigned int;

class IAccounting
{
public:
	virtual long unsigned int GetDeletedPlatformsCount() = 0;
	virtual long unsigned int GetAllBellowPlatformsCount(const int&) = 0;
	virtual void OnTopOfPlatformMessaging(long unsigned int) = 0;
	virtual void OnFlyingMessaging() = 0;
};
