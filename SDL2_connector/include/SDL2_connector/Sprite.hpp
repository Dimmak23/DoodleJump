#pragma once

//* Game

//* C++ std
#include <memory>

//* Forward declarations
struct IARectangle;
struct SDL_Rect;
struct SDL_Texture;
struct SDL_Renderer;

/*
@
*/
class Sprite
{
	//^Everything is private

	friend class Render;

	Sprite();
	Sprite(const Sprite& rhs) = delete;
	Sprite& operator=(const Sprite& rhs) = delete;
	~Sprite();

	//@ API: for render

	//* Getters
	SDL_Texture* GetTexture();
	SDL_Texture* GetCTexture() const;
	SDL_Rect* GetDestination() const;

	//* Setters
	void SetOrigin(int x, int y);
	void SetSize(int w, int h);

	//* Manipulators
	void Initiaze(const char* path, IARectangle* target_space);
	void ReleaseTextureMemory();

	//@ Members

	//* Geometry connection
	std::unique_ptr<SDL_Rect> _destination{ nullptr };

	//* Graphics
	SDL_Texture* _texture{ nullptr };
};
