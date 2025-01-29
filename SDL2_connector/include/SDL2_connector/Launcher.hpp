#pragma once

//* Forward declarations
class IApplication;

#ifdef _WIN32
using uint64_t = unsigned long long;
#else
#include <cstdint>
#endif

#ifdef BUILD_CONNECTOR_API
#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
// #define DLL_EXPORT __atribute__((visibility("default")))
#define DLL_EXPORT
#endif
#else
#ifdef _WIN32
#define DLL_EXPORT __declspec(dllimport)
#else
#define DLL_EXPORT
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

class Launcher
{
public:
	//@ API: for main.cpp

	//? Launch application
	//? returns '-1' when couldn't run
	static DLL_EXPORT int Run(IApplication* application);

	//@ API: for application.cpp

	//? Get the number of milliseconds since library initialization
	static DLL_EXPORT uint64_t GetTickCount();
	//? out parameters:
	//? screen size on windowed mode
	//? resolution size on fullscreen mode
	static DLL_EXPORT void GetScreenSize(int& width, int& height);

private:
	//@ Methods

	//? Return 'true' on successfully initialized SDL2
	static bool InitializeSDL2Features();
	static bool InitializeWindow(IApplication* application);
	static bool InitializeRenderer();
};

#ifdef __cplusplus
}
#endif
