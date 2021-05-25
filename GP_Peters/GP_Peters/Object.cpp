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
void Object::setImg(std::string filename, SDL_Renderer* renderer) {
	SDL_Surface* tmpSurf = IMG_Load()
}
