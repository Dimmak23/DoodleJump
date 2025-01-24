#pragma once

//* Game

//* C++ std
#include <memory>

//* Forward declarations
class RectangleCore;
class Sprite;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

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

class Render
{
	friend class Sprite;
	friend class Launcher;

public:
	//@ API

	//* Vendor connections
	static DLL_EXPORT Sprite* CreateSprite(const char* path);
	static DLL_EXPORT int DrawSprite(Sprite* sprite, int x, int y);
	static DLL_EXPORT void DestroySprite(Sprite** sprite);
	static DLL_EXPORT void GetSpriteSize(Sprite* sprite, int& x, int& y);
	static DLL_EXPORT void SetSpriteSize(Sprite* sprite, int x, int y);

private:
	//@ Methods

	//* Getters
	static SDL_Window* GetWindow();
	// static SDL_Renderer** getRenderer();
	static SDL_Renderer* GetRenderer();

	//* Setters
	static void SetWindow(SDL_Window* new_window);
	static void SetRenderer(SDL_Renderer* new_renderer);

	//* Manipulators
	static SDL_Surface* LoadSurface(const char* path);
	static void ReleaseRenderer();

	//@ Members

	//* Graphics
	static inline SDL_Window* _window{ nullptr };		 //? Will be initialized with Launcher
	static inline SDL_Renderer* _renderer{ nullptr };	 //? Will be initialized with Launcher
};

#ifdef __cplusplus
}
#endif
