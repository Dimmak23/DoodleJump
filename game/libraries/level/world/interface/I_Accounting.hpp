#pragma once

//* Forward declarations
using size_t = unsigned long long int;

class IAccounting
{
public:
	virtual size_t getDeletedPlatformsCount() = 0;
	virtual size_t getAllBellowPlatformsCount(const int&) = 0;
	virtual void onTopOfPlatformMessaging(size_t) = 0;
	virtual void onFlyingMessaging() = 0;
};
