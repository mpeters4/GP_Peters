#include "Game.h"
#include "TextureLoader.h"
#include <iostream>
#include <ctime>

using namespace std;

#define TILE_SIZE 32
#define MAX_JUMPTIME 2000

Object player;
bool jumpCharge = false;
Uint32 jumpTimer = 0;
const Uint8* keystate = SDL_GetKeyboardState(NULL);
float gravity = 2;
bool air = false;
bool jump = false;
bool colision = false;
bool groundCol = false;
//movement
float velX, velY;
float velDX, velDY;
//JUMP motion
float flPrevTime = 0;
float flCurTime = SDL_GetTicks();
float dt;

Game::Game() {}
Game::~Game() {

}

void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen) {
	int fs = 0;
	if (fullscreen) {
		fs = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, x, y, width, height, fs);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	initLevel();
	player.setDest(800, 704, 32, 32);
	player.setSrc(0, 0, 32, 32);
	player.setImg("model/test.png", renderer);
	
	
}

void Game::eventHandler() {
	SDL_Event e;
	SDL_PollEvent(&e);
	switch (e.type){
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:	
		if (e.key.keysym.sym == SDLK_RIGHT && !air &&!jumpCharge) {
			velDX = velX;
		}
		if (e.key.keysym.sym == SDLK_LEFT && !air && !jumpCharge) {
			velDX = -velX;
		}
		if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_RIGHT] && !air) {
			if (!jumpCharge) {
				velDX = 0;
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			else {
				if ((SDL_GetTicks() - jumpTimer) >= MAX_JUMPTIME) {
					velDY = -velY * (MAX_JUMPTIME * 0.0015);
					velDX = velX;
					jumpCharge = false;
				}
			}
		}
		if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_LEFT] && !air) {
			if (!jumpCharge ) {
				velDX = 0;
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}else  {
				if ((SDL_GetTicks() - jumpTimer) >= MAX_JUMPTIME && jumpCharge) {
					jumpCharge = false;
					velDY = -velY * (MAX_JUMPTIME * 0.0015);
					velDX = -velX;
					
				}
			}
		}

		if (e.key.keysym.sym == SDLK_SPACE && !air) {
			if (!jumpCharge && velDY == 0) {
				velDX = 0;
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			else {
				if ((SDL_GetTicks() - jumpTimer) >= MAX_JUMPTIME) {
					velDY = -velY * (MAX_JUMPTIME * 0.0015);
					jumpCharge = false;
				}
			}
		}
		break;
	case SDL_KEYUP:
		if (e.key.keysym.sym == SDLK_LEFT && !air) {
			velDX = 0;
		}
		else if (e.key.keysym.sym == SDLK_RIGHT && !air) {
			velDX = 0;
		}
		if (e.key.keysym.sym == SDLK_SPACE) {
			if (jumpCharge) {
				if (keystate[SDL_SCANCODE_LEFT] && !air) {
					velDX = -velX;
					velDY = - velY *((SDL_GetTicks() - jumpTimer) * 0.0015);
				}else if (keystate[SDL_SCANCODE_RIGHT] && !air) {
					velDX = velX;
					velDY = -velY * ((SDL_GetTicks() - jumpTimer) * 0.0015);
				}
				else if(!air) {
					velDY = -velY * ((SDL_GetTicks() - jumpTimer) * 0.0015);
				}
				jumpCharge = false;
			} 
		}
		break;
	default:
		break;
	}

}

void Game::draw(Object o) {
	SDL_Rect dest = o.getDest();
	SDL_Rect src = o.getSrc();
	SDL_RenderCopy(renderer, o.getTex(), &src, &dest);
	
}

void Game::update() {
	calcMovement();
	calcAir();
}



void Game::render() {
	SDL_RenderClear(renderer);
	drawMap();
	draw(player);
	SDL_RenderPresent(renderer);
	
}

void Game::initLevel() {
	velX = 1;
	velY = 1;
	loadMap("res/1.map");
}

void Game::loadMap(const char* filename) {
	int current, x, y, w, h;
	Object tmp;
	tmp.setImg("model/tiles.png", renderer);
	std::ifstream in(filename);
	if (!in.is_open()) {
		cerr << "ERROR wit loading file: " << filename << endl;
		return;
	}
	in >> w;
	in >> h;
	in >> x;
	in >> y;
	cout << w << endl << h << endl << x << endl << y << endl;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (in.eof()) {
				cerr << "Map reading ERROR" << endl;
				return;
			}
			in >> current;
			if (current != 0) {
				tmp.setSrc(((current - 1)*TILE_SIZE), 0, TILE_SIZE, TILE_SIZE);
				tmp.setDest((j*TILE_SIZE)+ x, (i*TILE_SIZE) + y, TILE_SIZE, TILE_SIZE);
				map.push_back(tmp);
			}
		}
	}
	in.close();
}

void Game::drawMap() {
	for (int i = 0; i < map.size(); i++) {
		draw(map[i]);
	}
}



bool Game::checkCollision(Object a, Object b) {
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of Obj A
	leftA = a.getDest().x;
	rightA = a.getDest().x + a.getDest().w;
	topA = a.getDest().y;
	bottomA = a.getDest().y + a.getDest().h;

	//Calculate the sides of Obj B
	leftB = b.getDest().x;
	rightB = b.getDest().x + b.getDest().w;
	topB = b.getDest().y;
	bottomB = b.getDest().y + b.getDest().h;
	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return 0;
	}

	if (topA >= bottomB)
	{
		return 0;
	}

	if (rightA <= leftB)
	{
		return 0;
	}

	if (leftA >= rightB)
	{
		return 0;
	}
	return 1;
	
}


void Game::calcMovement() {
	if (air) {
		/**/
		flPrevTime = flCurTime;
		flCurTime = SDL_GetTicks();
		dt = (flCurTime - flPrevTime) * 0.001;
		cout << "dt " << dt << endl;
		if (dt >= 0.007) {
			dt = 0.007;
			//dt = 0.03;
		}
		velDY = velDY + gravity * dt;
	}

	player.move(velDX, 0);
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1) {
			player.move(-velDX, 0);
			if (air) {
				velDX = velDX * -1;
			}
			
		}
	}
	player.move(0, velDY);
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1) {
			player.move(0, -velDY);
			if (velDY < 0) {
				velDY = velDY * -1;
			}
			else {
				groundCol = true;
			}
			
			
		}
	}
	if (groundCol) {
		player.move(velDX, 0);
		for (int i = 0; i < map.size(); i++) {
			if (checkCollision(player, map[i]) == 1) {
				player.move(-velDX, 0);
			}
		}
		while (!colision) {
			player.move(0, 1);
			for (int i = 0; i < map.size(); i++) {
				if (checkCollision(player, map[i]) == 1) {
					player.move(0, -1);
					colision = true;
				}
			}
		}
		colision = false;
		groundCol = false;
	}
}

void Game::calcAir() {
	player.move(0, 1);
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1) {
			colision = true;
		}
	}
	if (colision) {
		colision = false;
		if (air) {
			velDX = 0;
			velDY = 0;
			air = false;
		}
		
	}
	else {
		air = true;
	}
	player.move(0, -1);
}


void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

}