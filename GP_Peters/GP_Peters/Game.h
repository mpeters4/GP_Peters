#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Object.h"
#include "Player.h"

#include <stdio.h>
#include <vector>
#include <fstream>

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int x, int y, int width, int height, bool fullscreen);
	
	void eventHandler();
	void update();
	void render();
	void clean();
	void draw(Object o);
	void initLevel();
	void loadMap(const char* filename);
	void drawMap();
	void calcAir();
	void calcJump();
	void calcMovement();
	void playerAnimation();
	bool checkCollision(Object a, Object b);
	bool running() { return isRunning; };
	

private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
	int cnt = 0;
	std::vector<Object> map;
};