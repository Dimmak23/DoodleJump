#pragma once

//* Custom
//? Modules
//? Inheritance
//? Interfaces

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
	SDL_Texture* getTexture();
	SDL_Texture* getCTexture() const;
	SDL_Rect* getDestination() const;

	//* Setters
	void setOrigin(int x, int y);
	void setSize(int w, int h);

	//* Manipulators
	void initiaze(const char* path, IARectangle* target_space);
	void releaseTextureMemory();

	//@ Members

	//* Graphics
	SDL_Texture* _texture{ nullptr };

	//* Geometry connection
	std::unique_ptr<SDL_Rect> _Destination{ nullptr };
};
