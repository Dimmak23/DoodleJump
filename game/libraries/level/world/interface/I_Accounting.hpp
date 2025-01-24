#pragma once

//* Forward declarations
using size_t = unsigned long long int;

class IAccounting
{
public:
	virtual size_t GetDeletedPlatformsCount() = 0;
	virtual size_t GetAllBellowPlatformsCount(const int&) = 0;
	virtual void OnTopOfPlatformMessaging(size_t) = 0;
	virtual void OnFlyingMessaging() = 0;
};
