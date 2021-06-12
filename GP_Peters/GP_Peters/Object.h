#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Object {
private:
	SDL_Rect dest, src;
	//SDL_Rect src;
	SDL_Texture* tex;
	bool solid;
public:
	//SDL_Rect dest;
	Object() { solid = true; };
	SDL_Rect getDest() const { return dest; };
	SDL_Rect getSrc() const { return src; };
	SDL_Texture* getTex() const { return tex; };
	bool getSolid() const { return solid; }
	void setDest(int x, int y, int w, int h);
	void setSrc(int x, int y, int w, int h);
	void setImg(const char* filename, SDL_Renderer* renderer);
	void setSolid(bool solid);
	void move(int dir, float velocity);

};