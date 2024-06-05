//* Source header
#include "Launcher.hpp"

//* Vendor: SDL2
// #ifdef _WIN32
#include "SDL.h"
#include "SDL_image.h"
// #else
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #endif

//* Vendor connection
#include "IApplication.hpp"
#include "Loger.hpp"
#include "Render.hpp"

//* C++ std

int Launcher::Run(IApplication* application)
{
	//? Initialize all connected vendor features
	if (!Launcher::InitializeSDL2Features())
	{
		return -1;
	}

	//? Create window
	if (!Launcher::InitializeWindow(application))
	{
		IMG_Quit();
		SDL_Quit();
		return -1;
	}

	//? Create renderer
	if (!Launcher::InitializeRenderer())
	{
		SDL_DestroyWindow(Render::getWindow());
		IMG_Quit();
		SDL_Quit();
		return -1;
	}

	//? Here we can initialize
	application->Init();

	bool quit = false;
	std::unique_ptr<SDL_Event> _events = std::make_unique<SDL_Event>();

	// ? Application loop
	while (!quit)
	{
		//? Clear before any rendering
		SDL_RenderClear(Render::getRenderer());

		//? Here we play
		if (application->Tick())
		{
			break;
		}

		//? Enable after
		SDL_RenderPresent(Render::getRenderer());

		//* Event handling
		while (SDL_PollEvent(_events.get()))
		{
			if (_events->type == SDL_QUIT)	  //? OS specific close app button
			{
				quit = true;
			}

			//? Read keyboard pressed
			if (_events->type == SDL_KEYDOWN)
			{
				//? Keyboard close button
				if (_events->key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}

				//? Handle arrows
				if (_events->key.keysym.sym == SDLK_LEFT)
				{
					application->onKeyPressed(IAKey::LEFT);
				}
				if (_events->key.keysym.sym == SDLK_RIGHT)
				{
					application->onKeyPressed(IAKey::RIGHT);
				}
				if (_events->key.keysym.sym == SDLK_UP)
				{
					application->onKeyPressed(IAKey::UP);
				}
				if (_events->key.keysym.sym == SDLK_DOWN)
				{
					application->onKeyPressed(IAKey::DOWN);
				}
			}

			//? Read keyboard pressed
			if (_events->type == SDL_KEYUP)
			{
				//? Handle arrows
				if (_events->key.keysym.sym == SDLK_LEFT)
				{
					application->onKeyReleased(IAKey::LEFT);
				}
				if (_events->key.keysym.sym == SDLK_RIGHT)
				{
					application->onKeyReleased(IAKey::RIGHT);
				}
				if (_events->key.keysym.sym == SDLK_UP)
				{
					application->onKeyReleased(IAKey::UP);
				}
				if (_events->key.keysym.sym == SDLK_DOWN)
				{
					application->onKeyReleased(IAKey::DOWN);
				}
			}

			//? Did mouse have been moved?
			if (_events->type == SDL_MOUSEMOTION)
			{
				application->onMouseMove(					 //
					_events->motion.x, _events->motion.y,	 //
					_events->motion.xrel, _events->motion.yrel);
			}

			//? Did mouse button was clicked or released

			//? Clicked
			if (_events->type == SDL_MOUSEBUTTONDOWN)
			{
				if (_events->button.button == SDL_BUTTON_LEFT)
				{
					application->onMouseButtonClick(IAMouseButton::LEFT, false);
				}
				if (_events->button.button == SDL_BUTTON_MIDDLE)
				{
					application->onMouseButtonClick(IAMouseButton::MIDDLE, false);
				}
				if (_events->button.button == SDL_BUTTON_RIGHT)
				{
					application->onMouseButtonClick(IAMouseButton::RIGHT, false);
				}
			}

			//? Released
			if (_events->type == SDL_MOUSEBUTTONUP)
			{
				if (_events->button.button == SDL_BUTTON_LEFT)
				{
					application->onMouseButtonClick(IAMouseButton::LEFT, true);
				}
				if (_events->button.button == SDL_BUTTON_MIDDLE)
				{
					application->onMouseButtonClick(IAMouseButton::MIDDLE, true);
				}
				if (_events->button.button == SDL_BUTTON_RIGHT)
				{
					application->onMouseButtonClick(IAMouseButton::RIGHT, true);
				}
			}
		}
	}

	//? Here we release application resources
	application->Close();

	//? Release Renderer, Window and other SDL2 resources
	SDL_DestroyRenderer(Render::getRenderer());
	SDL_DestroyWindow(Render::getWindow());
	IMG_Quit();
	SDL_Quit();
	LogLine("--r--\tSDL features released...");

	return 0;
}

uint64_t Launcher::GetTickCount() { return SDL_GetTicks64(); }

void Launcher::GetScreenSize(int& width, int& height)
{
	// TODO: get resolution of the user screen
	//  SDL_DisplayMode DisplayMode;
	//  // if (SDL_GetCurrentDisplayMode(0, &DisplayMode) != 0)
	//  if (SDL_GetDesktopDisplayMode(0, &DisplayMode) != 0)
	//  {
	//  	LogLine("SDL_GetDesktopDisplayMode failed: ", SDL_GetError());
	//  }
	//  else
	//  {
	//  	// LogLine(DisplayMode.format);
	//  	w = DisplayMode.w;
	//  	h = DisplayMode.h;
	//  }

	if (Render::getRenderer())
	{
		SDL_GetRendererOutputSize(Render::getRenderer(), &width, &height);
		LogLine("Resolution-> width: ", width, ", height: ", height);
	}
	else
	{
		LogLine("Can't get size... No renderer found!");
	}
}

bool Launcher::InitializeSDL2Features()
{
	//? Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LogLine("Error: Couldn't initialize SDL Video = ", SDL_GetError());
		return false;
	}
	//? Loading SDL_Image dynamic libraries
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		LogLine("Couldn't load SDL_Image dynamic libraries...");
		return false;
	}
	return true;
}

bool Launcher::InitializeWindow(IApplication* application)
{
	//? Application geometry and window mode
	int WindowWidth, WindowHeight{};
	bool bIsFullScreen{ false };

	//? Preinitialize application and get screen size that we want to create
	application->PreInit(WindowWidth, WindowHeight, bIsFullScreen);

	Uint32 CurrentWindowFlags;

	if (!bIsFullScreen)
	{
		//? Create windowed mode with specific size
		CurrentWindowFlags = SDL_WINDOW_OPENGL;
	}
	else
	{
		//? Create fullscreen mode with specific resolution
		CurrentWindowFlags = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
	}
	//! Fix dissapearing textures issue, by setting to OPENGL
	//! SDL_WINDOW_OPENGL

	// SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP
	// SDL_WINDOW_RESIZABLE

	Render::setWindow(					  //
		SDL_CreateWindow(				  //
			application->GetTitle(),	  //? We need application title:
			SDL_WINDOWPOS_UNDEFINED,	  //
			SDL_WINDOWPOS_UNDEFINED,	  //
			WindowWidth, WindowHeight,	  // TODO: test... should be different resolutions on fullscreen
			CurrentWindowFlags));

	//! But also have to provide a hint, because Windows fallback to the Direct3D
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	if (Render::getWindow() == nullptr)
	{
		LogLine("Error: Couldn't create window = ", SDL_GetError());
		return false;
	}
	else
	{
		LogLine("--a--\tAssigned window...");
	}

	LogLine("Created window width size/resolution ---> ", WindowWidth, "x", WindowHeight);

	return true;
}

bool Launcher::InitializeRenderer()
{
	//? Create a renderer for GPU accelerated drawing
	Render::setRenderer(															//
		SDL_CreateRenderer(Render::getWindow(),										//
						   -1,														//
						   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)	//
	);
	//@ SDL_RENDERER_SOFTWARE -	the renderer is a software fallback
	//@ SDL_RENDERER_ACCELERATED - the renderer uses hardware acceleration
	//@ SDL_RENDERER_PRESENTVSYNC - present is synchronized with the refresh rate
	//@ SDL_RENDERER_TARGETTEXTURE - the renderer supports rendering to texture

	//?'SDL_RENDERER_PRESENTVSYNC'- synchronize frames with screen updates
	//? you get black screen without it
	//! Look into some way to improve FPS and have vertical syncronization

	// TODO: read return result
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");	 //? Enable linear texture filtering

	// Set 'nullptr' texture as a rendering target
	// SDL_SetRenderTarget(Render::getRenderer(), nullptr);
	//? Don't we need this after renderer creation?

	if (Render::getRenderer() == nullptr)
	{
		LogLine("Error: Couldn't create renderer = ", SDL_GetError());
		return false;
	}
	else
	{
		LogLine("--a--\tAssigned renderer...");
	}

	//? Ensure transparent graphics are drawn correctly.
	SDL_SetRenderDrawBlendMode(Render::getRenderer(), SDL_BLENDMODE_BLEND);

	//? Output the name of the render driver.
	SDL_RendererInfo rendererInfo;
	SDL_GetRendererInfo(Render::getRenderer(), &rendererInfo);
	// LogLine("Renderer: ", rendererInfo.name);
	// std::cout << "max_texture_width = " << rendererInfo.max_texture_width << std::endl;
	// std::cout << "max_texture_height = " << rendererInfo.max_texture_height << std::endl;
	// LogLine("flags: ", rendererInfo.flags);
	// LogLine("max_texture_width: ", rendererInfo.max_texture_width);
	// LogLine("max_texture_height: ", rendererInfo.max_texture_height);
	// LogLine("num_texture_formats: ", rendererInfo.num_texture_formats);
	// LogLine("texture_formats: ", rendererInfo.texture_formats);
	// std::cout << "num_texture_formats = " << rendererInfo.num_texture_formats << std::endl;
	// std::cout << "num_texture_formats: " << rendererInfo.texture_formats << std::endl;

	return true;
}
