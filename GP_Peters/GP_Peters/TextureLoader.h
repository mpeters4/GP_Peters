#pragma once
#include "Game.h"

class TextureLoader {

public:
	static SDL_Texture* loadTexture(const char* filename, SDL_Renderer* ren);
};