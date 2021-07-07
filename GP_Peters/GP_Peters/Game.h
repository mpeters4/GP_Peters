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
	bool checkCollision(Object a, Object b);
	bool running() { return isRunning; };
	
private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
	Player player;
	Object background;
	Mix_Music* bgm = NULL;
	int cnt = 0;
	std::vector<Object> map;
	int level,mapHeight = 0;
	//Object player;
	bool jumpCharge = false;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	bool air = false;
	bool jump = false;
	bool colision = false;
	bool groundCol = false;
	bool finish = false;
	//movement
	int velX, velY, velAir;
	float velDX, velDY, gravity;
	//JUMP motion
	float flPrevTime = 0;
	float flCurTime = SDL_GetTicks();
	float dt = 0;
	float time = 0;
	float startTime = SDL_GetTicks();
	Uint32 jumpTimer = 0;
};