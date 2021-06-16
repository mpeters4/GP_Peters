#include "Game.h"
#include "TextureLoader.h"
#include <iostream>
#include <ctime>

using namespace std;

#define TILE_SIZE 32

Object player;
bool jumpCharge = false;
Uint32 jumpTimer = 0;
const Uint8* keystate = SDL_GetKeyboardState(NULL);
int  velocity;
int gravity = 1;
bool air = true;
bool jump = false;
bool colision = false;
//movement
int mX, mY;
float velX, velY;
float mLeft, mRight, mUp, mDown = 0;
float jumpDir = 0;
//JUMP motion
float flPrevTime = 0;
float flCurTime = SDL_GetTicks();
float dt;
float jumpHeight = 0;

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
		if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_LEFT] && !air) {
			printf("LEFT and SPACE Keys Pressed.\n");
			if (!jumpCharge) {
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			if ((SDL_GetTicks() - jumpTimer) >= 3000) {
				std::cout << "SPACE AND LEFT hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "jump!" << endl;
				jumpCharge = false;
				if (!jump) {
					//player.move(2, TILE_SIZE * (SDL_GetTicks() - jumpTimer) * 0.001);
					jumpHeight = player.getDest().y - (TILE_SIZE * (SDL_GetTicks() - jumpTimer) * 0.001);
					cout << jumpHeight << endl;
					mX = -1;
					mY = -1;
					jump = true;
				}
			}
		}
		if (e.key.keysym.sym == SDLK_RIGHT && !air &&!jumpCharge) {
			std::cout << "RIGHT down" << "\n";
			mX = 1;
		}
		if (e.key.keysym.sym == SDLK_LEFT && !air && !jumpCharge) {
			std::cout << "LEFT down" << "\n";	
			//player.move(-1, 1);
			mX = -1;
		}
		if (e.key.keysym.sym == SDLK_UP && !air) {
			mY= -1;
		}
		if (e.key.keysym.sym == SDLK_DOWN && !air) {
			mY = 1;
		}
		
	
		if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_RIGHT && !air]) {
			printf("RIGHT and SPACE Keys Pressed.\n");
			//player.move(1, 32);
		}

		if (e.key.keysym.sym == SDLK_SPACE && !air) {
			std::cout << "SPACE down" << "\n";
			if (!jumpCharge) {
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
			}
			if ((SDL_GetTicks() - jumpTimer) >= 3000) {
				std::cout << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "jump! " <<  endl;
				if (!jump) {
					//player.move(2, TILE_SIZE * (SDL_GetTicks() - jumpTimer) * 0.001);
					jumpHeight = player.getDest().y- (TILE_SIZE * (SDL_GetTicks() - jumpTimer) * 0.001) ;
					cout << jumpHeight <<endl;
					jump = true;
					mY = -1;
				}
			}
		}
		break;
	case SDL_KEYUP:
		if (e.key.keysym.sym == SDLK_LEFT && !air) {
			std::cout << "LEFT up" << "\n";
			mX = 0;
		}
		else if (e.key.keysym.sym == SDLK_RIGHT && !air) {
			std::cout << "RIGHT up" << "\n";
			mX = 0;
		}
		if (e.key.keysym.sym == SDLK_SPACE) {
			std::cout << "SPACE up" << "\n";
			if (jumpCharge) {
				if (keystate[SDL_SCANCODE_LEFT] && !air) {
					mX = -1;
				}
				std::cout << keystate[SDL_SCANCODE_LEFT] << "SPACE hold down for " << SDL_GetTicks() - jumpTimer << " Miliseconds" << endl << "Jumpheight: " << TILE_SIZE*(SDL_GetTicks() - jumpTimer) * 0.001 << endl;
				if (!jump) {
					//player.move(2, TILE_SIZE * (SDL_GetTicks() - jumpTimer) * 0.001);
					jumpHeight = player.getDest().y - (TILE_SIZE * (SDL_GetTicks() - jumpTimer) * 0.001) ;
					jump = true;
					mY = -1;
				}
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
	cout << keystate[SDL_SCANCODE_LEFT] << endl;
	calcMovement();
	calcAir();
	colision = false;

	if (air) {
		cout << "AIR " << air <<endl;
	}
	else {
		cout << "GROUND" << endl;
		jump = false;
	}
	

}

void Game::calcMovement() {
	cout << "CALCMOVEMENT" << endl;
	if (!jump) {
		player.newMove(mX, mY, velX, velY);
		for (int i = 0; i < map.size(); i++) {
			if (checkCollision(player, map[i]) == 1) {
				colision = true;
			}
		}
		if (colision) {
			player.newMove(-mX, -mY, velX, velY);
			colision = false;
		}
		
	}
	

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
	
	//If none of the sides from A are outside B
	cout << "col" << endl;
	return 1;
	
}

void Game::calcAir() {
	flPrevTime = flCurTime;
	flCurTime = SDL_GetTicks();
	dt = (flCurTime - flPrevTime) * 0.01;
	//cout << "dt " << dt << endl;
	if (dt >= 0.15) {
		dt = 0.15;
	}

	if (jump) {
		calcJump();
	}
	//gravity
	player.newMove(0, gravity, velX, velY * dt);
	//player.moveY(gravity, velY);
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1) {
			colision = true;
			cout << i << endl;
			air = false;	
		}
	}
	if (colision) {
		cout << "?" << endl;
		player.newMove(0, -gravity, velX, velY *dt);
		colision = false;
		//player.moveY(-gravity, velY);
	}
	else {
		air = true;
	}

}


void Game::calcJump() {
	cout << "JUMP MX = " << mX << " MY " << mY<< endl;
	player.newMove(mX, mY,  velX , velY);
	//player.moveX(mX, velX * dt);
	//player.moveY(mY, velY * dt);
	if (player.getDest().y <= jumpHeight) {
		cout << "HIGHEST POINT REACHED" << endl;
		//jump = false;
		jumpCharge = false;
		mY = 0;
	}
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1) {
			colision = true;

		}
	}
	if (colision) {
		
		player.newMove(- mX, -mY, velX * dt, velY * dt);
		mX = -mX;
		colision = false;
	}
	
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

}