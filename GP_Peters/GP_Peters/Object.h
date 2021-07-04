#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Object {
private:
	float velocityX, velocityY, posX, posY;
	SDL_Rect dest, src;
	//SDL_Rect src;
	SDL_Texture* tex;
	bool solid;
	bool finish;
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
	void moveX(float mX, float velocity);
	void moveY(float mY, float velY);
	void setPos(int x, int y);
	void move(float velX, float velY);
	void setFinish(bool end);
	bool getFinish()const { return finish; };

};
