#include "Object.h"

void Object::setDest(int x, int y, int w, int h) {
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	posX = x;
	posY = y;
	

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

void Object::setFinish(bool end) {
	Object::finish = end;
}

//
void Object::moveX(float mX, float velX) {
	posX += velX * mX;
	dest.x = posX;
}

void Object::moveY(float mY, float velY) {
	posY += velY * mY;
	dest.y = posY;
}

void Object::setPos(int x, int y){
	dest.x = x;
	dest.y = y;
	posX = x;
	posY = y;

}

void Object::move(float velX, float velY) {
	posX += velX;
	posY += velY;
	dest.x = posX;
	dest.y = posY;
}

