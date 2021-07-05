#include "TextureLoader.h"

SDL_Texture* TextureLoader::loadTexture(const char* texture, SDL_Renderer* ren){
	SDL_Surface* tempSur = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSur);
	SDL_FreeSurface(tempSur);
	return tex;
}