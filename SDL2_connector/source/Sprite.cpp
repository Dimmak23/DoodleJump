//* Source header
#include "Sprite.hpp"

//* Vendor: SDL2
#ifdef _WIN32
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

//* SDL2: connector
#include "IApplication.hpp"
#include "Loger.hpp"
#include "Render.hpp"

//* C++ std

Sprite::Sprite() { LogLine("--c--\tSprite object constructed on heap with size: ", sizeof(*this)); }

Sprite::~Sprite()
{
	ReleaseTextureMemory();
	LogLine("--d--\tSprite object destroyed...");
}

SDL_Texture* Sprite::GetTexture() { return _texture; }

SDL_Texture* Sprite::GetCTexture() const { return _texture; }

SDL_Rect* Sprite::GetDestination() const { return _destination.get(); }

void Sprite::SetOrigin(int x, int y)
{
	_destination->x = x;
	_destination->y = y;
}

void Sprite::SetSize(int width, int height)
{
	_destination->w = width;
	_destination->h = height;
}

void Sprite::Initiaze(const char* path, IARectangle* target_space)
{
	if ((Render::GetRenderer() == nullptr))
	{
		LogLine("No renderer...");
		return;
	}

	_destination = std::make_unique<SDL_Rect>();
	SDL_Rect* target = new SDL_Rect();
	SDL_Surface* tempSource = Render::LoadSurface(path);

	if (tempSource == nullptr)
	{
		LogLine("Image surface could not be created! SDL Error: ",	  //! Not explicit enough. Tell if file missing
				SDL_GetError()										  //
		);
		return;
	}

	bool __laterRelease{ false };
	if (target_space == nullptr)
	{
		//? Assign from temporary surface, if no target
		__laterRelease = true;
		target->x = 0;				  // X coordinate of the top-left corner of the cropped area
		target->y = 0;				  // Y coordinate of the top-left corner of the cropped area
		target->w = tempSource->w;	  // Width of the cropped area
		target->h = tempSource->h;	  // Height of the cropped area}
									  //! maybe call parent method for this

		_destination->x = 0;
		_destination->y = 0;
		_destination->w = tempSource->w;
		_destination->h = tempSource->h;
	}
	else
	{
		//? Re-assign inner ractangle by target, if any target exist
		_destination->x = target_space->x;
		_destination->y = target_space->y;
		_destination->w = target_space->w;
		_destination->h = target_space->h;

		target->w = target_space->w;
		target->h = target_space->h;
	}

	//? Create cropped image
	SDL_Surface* croppedSurface = SDL_CreateRGBSurface(	   //
		0,												   //
		target->w, target->h,							   //! resulted size
		tempSource->format->BitsPerPixel,				   //
		tempSource->format->Rmask,						   //
		tempSource->format->Gmask,						   //
		tempSource->format->Bmask,						   //
		tempSource->format->Amask						   //
	);

	SDL_SetClipRect(tempSource, target);
	SDL_BlitSurface(tempSource, target, croppedSurface, nullptr);	 //! Do text textures need this?

	if (__laterRelease) delete target;

	SDL_FreeSurface(tempSource);

	//? Create a texture from the surface
	_texture = SDL_CreateTextureFromSurface(Render::GetRenderer(), croppedSurface);

	if (_texture == nullptr)
	{
		LogLine("SDL_CreateTextureFromSurface(*Render::getRenderer(), croppedSurface) returned nullptr");
	}
	else
	{
		// LogLine("SDL_CreateTextureFromSurface PointerToPointer texture: ", _texture);
		LogLine("--a--\tAssigned texture...");
	}

	SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);					  //! Check return value
	SDL_SetTextureScaleMode(_texture, SDL_ScaleMode::SDL_ScaleModeLinear);	  //! Check return value

	SDL_FreeSurface(croppedSurface);
}

void Sprite::ReleaseTextureMemory()
{
	SDL_DestroyTexture(_texture);
	_texture = nullptr;
	LogLine("--r--\tTexture released...");
}
