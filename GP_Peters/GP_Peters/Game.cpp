#include "Game.h"
#include "TextureLoader.h"
#include <iostream>
#include <ctime>

using namespace std;

#define TILE_SIZE 32
#define MAX_JUMPTIME 3000

Object player;
bool jumpCharge = false;
Uint32 jumpTimer = 0;
const Uint8* keystate = SDL_GetKeyboardState(NULL);
int gravity = 1;
bool air = false;
bool jump = false;
bool colision = false;
bool groundCol = false;
//movement
int mX, mY;
float velX, velY;
float velDX, velDY;
float mLeft, mRight, mUp, mDown = 0;
float jumpDir = 0;
//JUMP motion
float flPrevTime = 0;
float flCurTime = SDL_GetTicks();
float dt;
float jumpHeight = 0;

int failDTCount = 0;

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

	
	player.setDest(800, 704, 32, 32);
	player.setSrc(0, 0, 32, 32);
	player.setImg("model/test.png", renderer);
	initLevel();
	
}

void Game::eventHandler() {
	SDL_Event e;
	SDL_PollEvent(&e);
	switch (e.type){
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:	
		/*if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_LEFT] && !air) {
			printf("LEFT and SPACE Keys Pressed.\n");
			if (!jumpCharge) {
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			if ((SDL_GetTicks() - jumpTimer) >= 3000) {
				std::cout << "SPACE AND LEFT hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl ;
				jumpCharge = false;
				velDY = -(MAX_JUMPTIME * 0.001);
				//velDY= -((SDL_GetTicks() - jumpTimer) * 0.001);

				velDX = -velX;
				
			}
		}*/
		if (e.key.keysym.sym == SDLK_RIGHT && !air &&!jumpCharge) {
			std::cout << "RIGHT down" << "\n";

			velDX = velX;
		}
		if (e.key.keysym.sym == SDLK_LEFT && !air && !jumpCharge) {
			std::cout << "LEFT down" << "\n";	
			velDX = -velX;
		}
		if (e.key.keysym.sym == SDLK_UP && !air) {

			velDY = -velY;
		}
		if (e.key.keysym.sym == SDLK_DOWN && !air) {

			velDY = velY;
		}

	
		if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_RIGHT] && !air) {
			if (!jumpCharge) {
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			else {
				if ((SDL_GetTicks() - jumpTimer) >= MAX_JUMPTIME) {
					std::cout << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "jump! " << endl;
					velDY = -(MAX_JUMPTIME * 0.001);
					std::cout << "SPACE AND RIGHT hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "VEL: " << -((SDL_GetTicks() - jumpTimer) * 0.001) << endl;
					velDX = velX;
					jumpCharge = false;
				}
			}
		}
		if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_LEFT] && !air) {
			if (!jumpCharge) {
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			else {
				if ((SDL_GetTicks() - jumpTimer) >= MAX_JUMPTIME) {
					std::cout << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "jump! " << endl;
					velDY = -(MAX_JUMPTIME * 0.001);
					std::cout << "SPACE AND RIGHT hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "VEL: " << -((SDL_GetTicks() - jumpTimer) * 0.001) << endl;
					velDX = -velX;
					jumpCharge = false;
				}
			}
		}

		if (e.key.keysym.sym == SDLK_SPACE && !air) {
			std::cout << "SPACE down" << "\n";
			if (!jumpCharge) {
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			else {
				if ((SDL_GetTicks() - jumpTimer) >= MAX_JUMPTIME) {
					std::cout << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "jump! " << endl;
					velDY = -(MAX_JUMPTIME * 0.001);
					jumpCharge = false;
				}
			}
		}
		break;
	case SDL_KEYUP:
		if (e.key.keysym.sym == SDLK_LEFT && !air) {
			std::cout << "LEFT up" << "\n";
			//mX = 0;
			velDX = 0;
		}
		else if (e.key.keysym.sym == SDLK_RIGHT && !air) {
			std::cout << "RIGHT up" << "\n";
			//mX = 0;
			velDX = 0;
		}
		if (e.key.keysym.sym == SDLK_UP && !air) {
			//mY = 0;
			velDY = 0;
		}
		if (e.key.keysym.sym == SDLK_DOWN && !air) {
			//mY = 0;
			velDY = 0;
		}
		if (e.key.keysym.sym == SDLK_SPACE) {
			std::cout << "SPACE up" << "\n";
			if (jumpCharge) {
				if (keystate[SDL_SCANCODE_LEFT] && !air) {
					std::cout <<  "SPACE AND LEFT hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "VEL: " << -((SDL_GetTicks() - jumpTimer) * 0.001) << endl;
					velDX = -velX;
					velDY = -((SDL_GetTicks() - jumpTimer) * 0.001);
				}else if (keystate[SDL_SCANCODE_RIGHT] && !air) {
					std::cout << "SPACE AND RIGHT hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "VEL: " << -((SDL_GetTicks() - jumpTimer) * 0.001) << endl;
					velDX = velX;
					velDY = -((SDL_GetTicks() - jumpTimer) * 0.001);
				}
				else {
					std::cout << keystate[SDL_SCANCODE_LEFT] << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "Jumpheight: " << TILE_SIZE*(SDL_GetTicks() - jumpTimer) * 0.001 << endl;
					velDY = -((SDL_GetTicks() - jumpTimer) * 0.001);
					
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
	//SDL_RenderCopyEx(renderer, o.getTex(), &src, &dest, 0,NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, o.getTex(), &src, &dest);
	
}

void Game::update() {
	calcMovement();
	calcAir();
	colision = false;
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
	loadMap("res/test.map");
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
		//cout << "drawing tile on x: " << map[i].getDest().x << " y: " << map[i].getDest().y << endl;
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
		if (dt >= 0.015) {
			dt = 0.015;
			
		}
		else {
			failDTCount += 1;
		}cout << "FAIL DT " << failDTCount << endl;
		velDY = velDY + gravity * dt;
		cout << "VELDX " << velDX << " VELDY " << velDY << endl;
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
			cout << "HARDRESET" << endl;
			velDX = 0;
			velDY = 0;
			air = false;
		}
		
	}
	else {
		air = true;
	}
	player.move(0, -1);


	/*
	cout << "JH " << jumpHeight << endl;
	flPrevTime = flCurTime;
	flCurTime = SDL_GetTicks();
	dt = (flCurTime - flPrevTime) * 0.001;
	cout << "dt " << dt << endl;
	if (dt >= 0.016) {
		dt = 0.016;

		cout << "VELDX " << velDX << " VELDY " << velDY << endl;
	}
	if (player.getDest().y <= jumpHeight) {
		cout << "HIGHEST POINT REACHED" << endl;
		jump = false;
		jumpCharge = false;
		//mY = 1;
	}
	if (air) {
		velDX = velDX * dt;
		velDY = velDY + velDY * dt;
		//calcJump();
	}
	else {
		cout << "VEL RESET" << endl;
		velDX = 0;
		velDY = 0;
	}
	//gravity
	player.newMove(0, gravity, 1, 1);
	//player.moveY(gravity, velY);
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1) {
			colision = true;
			air = false;
		}
	}
	if (colision) {
		player.newMove(0, -gravity, 1, 1);
		colision = false;
	}
	else {
		air = true;
	}*/

}


void Game::calcJump() {
	cout << "JUMP MX = " << mX << " MY " << mY<< endl;
	player.newMove(mX, mY,  velDX , velDY);
	//player.moveX(mX, velX * dt);
	//player.moveY(mY, velY * dt);
	if (player.getDest().y <= jumpHeight) {
		cout << "HIGHEST POINT REACHED" << endl;
		jump = false;
		jumpCharge = false;
		mY = 0;
	}
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1) {
			colision = true;

		}
	}
	if (colision) {
		cout << "JUMPCOL" << endl;
		player.newMove(- mX, -mY, velDX * dt, velDY * dt);
		colision = false;
		if (air) {
			mX = -mX;
		}
	}
	
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

}