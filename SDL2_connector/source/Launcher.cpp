//* Source header
#include "Launcher.hpp"

//* Vendor: SDL2
#include "SDL.h"
#include "SDL_image.h"

//* SDL2: connector
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
		SDL_DestroyWindow(Render::GetWindow());
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
		SDL_RenderClear(Render::GetRenderer());

		//? Here we play
		// ? It's GameApplication (which implements IApplication class) responsibility
		// ? to return true when game should be over by game reasons (lifes exceeded, etc.)
		if (application->Tick())
		{
			break;
		}

		//? Enable after
		SDL_RenderPresent(Render::GetRenderer());

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
					application->OnKeyPressed(IAKey::LEFT);
				}
				if (_events->key.keysym.sym == SDLK_RIGHT)
				{
					application->OnKeyPressed(IAKey::RIGHT);
				}
				if (_events->key.keysym.sym == SDLK_UP)
				{
					application->OnKeyPressed(IAKey::UP);
				}
				if (_events->key.keysym.sym == SDLK_DOWN)
				{
					application->OnKeyPressed(IAKey::DOWN);
				}
			}

			//? Read keyboard pressed
			if (_events->type == SDL_KEYUP)
			{
				//? Handle arrows
				if (_events->key.keysym.sym == SDLK_LEFT)
				{
					application->OnKeyReleased(IAKey::LEFT);
				}
				if (_events->key.keysym.sym == SDLK_RIGHT)
				{
					application->OnKeyReleased(IAKey::RIGHT);
				}
				if (_events->key.keysym.sym == SDLK_UP)
				{
					application->OnKeyReleased(IAKey::UP);
				}
				if (_events->key.keysym.sym == SDLK_DOWN)
				{
					application->OnKeyReleased(IAKey::DOWN);
				}
			}

			//? Did mouse have been moved?
			if (_events->type == SDL_MOUSEMOTION)
			{
				application->OnMouseMove(					 //
					_events->motion.x, _events->motion.y,	 //
					_events->motion.xrel, _events->motion.yrel);
			}

			//? Did mouse button was clicked or released

			//? Clicked
			if (_events->type == SDL_MOUSEBUTTONDOWN)
			{
				if (_events->button.button == SDL_BUTTON_LEFT)
				{
					application->OnMouseButtonClick(IAMouseButton::LEFT, false);
				}
				if (_events->button.button == SDL_BUTTON_MIDDLE)
				{
					application->OnMouseButtonClick(IAMouseButton::MIDDLE, false);
				}
				if (_events->button.button == SDL_BUTTON_RIGHT)
				{
					application->OnMouseButtonClick(IAMouseButton::RIGHT, false);
				}
			}

			//? Released
			if (_events->type == SDL_MOUSEBUTTONUP)
			{
				if (_events->button.button == SDL_BUTTON_LEFT)
				{
					application->OnMouseButtonClick(IAMouseButton::LEFT, true);
				}
				if (_events->button.button == SDL_BUTTON_MIDDLE)
				{
					application->OnMouseButtonClick(IAMouseButton::MIDDLE, true);
				}
				if (_events->button.button == SDL_BUTTON_RIGHT)
				{
					application->OnMouseButtonClick(IAMouseButton::RIGHT, true);
				}
			}
		}
	}

	//? Here we release application resources
	application->Close();

	//? Release Renderer, Window and other SDL2 resources
	SDL_DestroyRenderer(Render::GetRenderer());
	SDL_DestroyWindow(Render::GetWindow());
	IMG_Quit();
	SDL_Quit();
	LogLine("--r--\tSDL features released...");

	return 0;
}

uint64_t Launcher::GetTickCount() { return SDL_GetTicks64(); }

void Launcher::GetScreenSize(int& width, int& height)
{
	if (Render::GetRenderer())
	{
		SDL_GetRendererOutputSize(Render::GetRenderer(), &width, &height);
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

	Render::SetWindow(					  //
		SDL_CreateWindow(				  //
			application->GetTitle(),	  //? We need application title:
			SDL_WINDOWPOS_UNDEFINED,	  //
			SDL_WINDOWPOS_UNDEFINED,	  //
			WindowWidth, WindowHeight,	  // TODO: test... should be different resolutions on fullscreen
			CurrentWindowFlags));

	//! But also have to provide a hint, because Windows fallback to the Direct3D
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	if (Render::GetWindow() == nullptr)
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
	Render::SetRenderer(																  //
		SDL_CreateRenderer(Render::GetWindow(),											  //
						   -1,															  //
						   SDL_RENDERER_ACCELERATED /* | SDL_RENDERER_PRESENTVSYNC */)	  //
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

	if (Render::GetRenderer() == nullptr)
	{
		LogLine("Error: Couldn't create renderer = ", SDL_GetError());
		return false;
	}
	else
	{
		LogLine("--a--\tAssigned renderer...");
	}

	//? Ensure transparent graphics are drawn correctly.
	SDL_SetRenderDrawBlendMode(Render::GetRenderer(), SDL_BLENDMODE_BLEND);

	//? Output the name of the render driver.
	SDL_RendererInfo rendererInfo;
	SDL_GetRendererInfo(Render::GetRenderer(), &rendererInfo);

	return true;
}
