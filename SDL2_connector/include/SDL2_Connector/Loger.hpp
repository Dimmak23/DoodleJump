#pragma once

#if defined(_DEBUG)

#include <iostream>

inline void LogLine() { std::cout << '\n'; }

template <typename First, typename... Args>
inline void LogLine(First first, Args... args)
{
	std::cout << first;
	LogLine(args...);
}

#else

template <typename First, typename... Args>
inline void LogLine(First first, Args... args)
{}

#endif
