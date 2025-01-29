//* Source header
#include "Render.hpp"

//* Vendor: SDL2
#include "SDL.h"
#include "SDL_image.h"

//* SDL2: connector
#include "Loger.hpp"
#include "Sprite.hpp"

//* C++ std
#include <string_view>

Sprite* Render::CreateSprite(const char* path)
{
	Sprite* sprite = new Sprite();
	sprite->Initiaze(path, nullptr);
	return sprite;
}

SDL_Window* Render::GetWindow() { return _window; }

SDL_Renderer* Render::GetRenderer() { return _renderer; }

void Render::SetWindow(SDL_Window* new_window) { _window = new_window; }

void Render::SetRenderer(SDL_Renderer* new_renderer) { _renderer = new_renderer; }

SDL_Surface* Render::LoadSurface(const char* path)
{
	std::string_view temporary{ path };

	//? Need at least 5 characters: x.ext, x - name, '.' - delimeter, ext - extension
	if (temporary.length() < 5)
	{
		LogLine("Couldn't load image by path: ", path);
		LogLine("Image path to short, need 5 characters...");
		return nullptr;
	}

	//? View last three characters for extension
	if (temporary.ends_with(".png"))
	{
		return IMG_Load(path);
	}
	else if (temporary.ends_with(".bmp"))
	{
		return SDL_LoadBMP(path);
	}
	else
	{
		LogLine("Couldn't load image by path: ", path);
		LogLine("Not png or bmp...");
		return nullptr;	   //! Haven't tested cases other then *.png or *.bmp
	}
}

void Render::ReleaseRenderer() { SDL_DestroyRenderer(_renderer); }

//?Such approach more generous, we can apply rotation and flipping if we want to
//* Render with rotation angle and flipping
int Render::DrawSprite(Sprite* sprite, int x, int y)
{
	if ((_renderer == nullptr) || (sprite->GetTexture() == nullptr))
	{
		LogLine("*_parent and/or *_texture pointers assigned to 'nullptr'... ");
		LogLine("Issue in the Texture::copy() method");
		return -1;
	}

	// SDL_SetTextureAlphaMod(*_texture, _opacity); //! Need Transparent class
	// SDL_SetTextureAlphaMod(sprite->getTexture(), 255);

	sprite->SetOrigin(x, y);

	//^ Returns 0 on success or a negative error code on failure; call SDL_GetError() for more information.
	int __render_result =
		SDL_RenderCopyEx(_renderer,					//
						 (sprite->GetTexture()),	//
						 nullptr,					//    the source SDL_Rect structure
						 //				    or nullptr for the entire texture
						 //? if destination rectangle have been parsed: copy to it
						 //? if not: copy to it's self position
						 sprite->GetDestination(),	  //  the destination SDL_Rect structure
						 //			    	or NULL for the entire rendering target
						 0,	   //		an angle in degrees that indicates the rotation that will be applied to
							   // dstrect,
						 //				    rotating it in a clockwise direction
						 nullptr,	 //		a pointer to a point indicating the point around which
									 // dstrect will be rotated
						 //				    (if NULL, rotation will be done around `dstrect.w / 2`, `dstrect.h / 2`)
						 SDL_FLIP_NONE	  // a SDL_RendererFlip value stating which flipping actions
										  // should be performed on the texture
		);

	if (__render_result)
	{
		LogLine("Failed to render texture: ", sprite->GetTexture(), ", ", SDL_GetError());
	}

	return __render_result;
}

void Render::DestroySprite(Sprite** sprite)
{
	if (*sprite)
	{
		delete *sprite;
		*sprite = nullptr;
	}
}

void Render::GetSpriteSize(Sprite* sprite, int& width, int& height)
{
	width = sprite->GetDestination()->w;
	height = sprite->GetDestination()->h;
}

void Render::SetSpriteSize(Sprite* sprite, int width, int height) { sprite->SetSize(width, height); }
