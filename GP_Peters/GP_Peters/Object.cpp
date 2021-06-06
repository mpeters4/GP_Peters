#include "Object.h"

void Object::setDest(int x, int y, int w, int h) {
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;

}
void Object::setSrc(int x, int y, int w, int h) {
	src.x = x;
	src.y = y;
	src.w = w;
	src.h = h;
}

void Object::setImg(const char* filename, SDL_Renderer* renderer) {
	SDL_Surface* tmpSurf = IMG_Load(filename);
	//tex = SDL_CreateTextureFromSurface(renderer, tmpSurf);
	tex = IMG_LoadTexture(renderer, filename);
	SDL_FreeSurface(tmpSurf);
}

void Object::setSolid(bool solid) {
	Object::solid = solid;
}

void Object::move(int dir) {
	if (dir == 1) {
		dest.x++;
	}
	if (dir == -1) {
		dest.x--;
	}
	if (dir == 0) {
		dest.y += 20;
	}

}