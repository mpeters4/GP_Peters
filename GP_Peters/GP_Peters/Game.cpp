#include "Game.h"
#include "TextureLoader.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <SDL_mixer.h>

using namespace std;

#define TILE_SIZE 32
#define MAX_JUMPTIME 1000


//ANIMATIONS
int idolL, idolR ,runL,runR ,jumpChargeL ,jumpChargeR ,jumpL ,jumpR;

Game::Game() {
	velX = 0;
	velY = 0;
	velAir = 0;
	velDX = 0;
	velDY = 0;
	gravity = 0;
	isRunning = false;
	level = 0;
	renderer = NULL;
	window = NULL;
}
Game::~Game() {
}

void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen) {
	int fs = 0;
	mapHeight = height;
	if (fullscreen) {
		fs = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, x, y, width, height, fs);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			isRunning = true;
			//LEVEL
			level = 7;
			background.setDest(0, 0, width, height);
			background.setSrc(0, 0, width, height);
			background.setImg("model/background.png", renderer);
			player.setDest(120, 684, 28, 58);
			//player.setDest(300, 300, 28, 58);
			player.setSrc(0, 0, 28, 58);
			player.setImg("model/GP_Player_sprite.png", renderer);
			player.setCurAnimation(idolR);
			idolL = player.createCycle(0, player.getSrc().w, player.getSrc().h, 2, 50);
			idolR = player.createCycle(1, player.getSrc().w, player.getSrc().h, 2, 50);
			runL = player.createCycle(2, player.getSrc().w, player.getSrc().h, 4, 5);
			runR = player.createCycle(3, player.getSrc().w, player.getSrc().h, 4, 5);
			jumpChargeL = player.createCycle(4, player.getSrc().w, player.getSrc().h, 1, 20);
			jumpChargeR = player.createCycle(5, player.getSrc().w, player.getSrc().h, 1, 20);
			jumpL = player.createCycle(6, player.getSrc().w, player.getSrc().h, 1, 20);
			jumpR = player.createCycle(7, player.getSrc().w, player.getSrc().h, 1, 20);
			initLevel();		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			cout << "ERROR:" << Mix_GetError() << endl;
		}
		else {
			bgm =Mix_LoadMUS("res/background.mp3");
			Mix_PlayMusic(bgm, -1);
			player.setSounds(Mix_LoadWAV("res/jump.wav"), Mix_LoadWAV("res/step.wav"), Mix_LoadWAV("res/colision.wav"));
		}
	}
	else {
		isRunning = false;
	}	
}

void Game::eventHandler() {
	SDL_Event e;
	SDL_PollEvent(&e);
	switch (e.type){
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:	
		if (keystate[SDL_SCANCODE_RIGHT] && !air && !jumpCharge) {
			velDX = velX;
			if (player.getCurAnimation() != runR) {
				player.setCurAnimation(runR);
			}
		}
		if (keystate[SDL_SCANCODE_LEFT] && !air && !jumpCharge) {
			velDX = -velX;
			if (player.getCurAnimation() != runL) {
				player.setCurAnimation(runL);
			}	
		}

		if (keystate[SDL_SCANCODE_SPACE] && !air) {
			if (!jumpCharge && velDY == 0) {
				velDX = 0;
				jumpTimer = SDL_GetTicks();
				jumpCharge = true;
				if (player.getCurAnimation() % 2 == 0) {
					player.setCurAnimation(jumpChargeL);
				}
				else {
					player.setCurAnimation(jumpChargeR);
				}
			}
		}
		break;
	case SDL_KEYUP:
		if (e.key.keysym.sym == SDLK_LEFT && !air && !jumpCharge) {
			velDX = 0;
			player.setCurAnimation(idolL);
		}
		else if (e.key.keysym.sym == SDLK_RIGHT && !air && !jumpCharge) {
			velDX = 0;
			player.setCurAnimation(idolR);
		}
		if (e.key.keysym.sym == SDLK_SPACE) {
			if (jumpCharge) {
				if (keystate[SDL_SCANCODE_LEFT] && !air) {
					velDX = -velAir;
				}else if (keystate[SDL_SCANCODE_RIGHT] && !air) {
					velDX = velAir;
				}
				velDY = -velY * ((SDL_GetTicks() - jumpTimer)*2 * 0.0015);
				jumpCharge = false;
				Mix_PlayChannel(-1, player.getSoundJump(), 0);
				
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
	time = (SDL_GetTicks() - startTime) / 1000/60;
	cout << "Time " << time << endl;
	if (player.getDest().y < 0) {
		level++;
		player.setPos(player.getDest().x, mapHeight);
		initLevel();
	}
	else if (player.getDest().y > mapHeight) {
		level--;
		player.setPos(player.getDest().x, 0);
		initLevel();
	}
	if (jumpCharge && keystate[SDL_SCANCODE_LEFT]) {
		player.setCurAnimation(jumpChargeL);
	}
	if (jumpCharge && keystate[SDL_SCANCODE_RIGHT]) {
		player.setCurAnimation(jumpChargeR);
	}
	calcMovement();
	calcAir();
	player.updateAnimation();
}



void Game::render() {
	SDL_RenderClear(renderer);
	draw(background);
	draw(player);
	drawMap();
	SDL_RenderPresent(renderer);
	
}

void Game::initLevel() {
	ostringstream os;
	os << "res/" << level << ".map";
	loadMap(os.str().c_str());
}

void Game::loadMap(const char* filename) {
	map.clear();
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
	in >> velX;
	velAir = velX * 1.5;
	in >> velY;
	in >> gravity;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (in.eof()) {
				cerr << "Map reading ERROR" << endl;
				return;
			}
			in >> current;
			if (current != 0) {
				tmp.setSrc(((current %10 )*TILE_SIZE), ((current/10))*TILE_SIZE, TILE_SIZE, TILE_SIZE);
				tmp.setDest((j*TILE_SIZE)+ x, (i*TILE_SIZE) + y, TILE_SIZE, TILE_SIZE);
				if (current > 39) {
					tmp.setSolid(false);
				}
				else {
					tmp.setSolid(true);
				}
				if (current == 59) {
					tmp.setFinish(true);
					tmp.setSolid(true);
				}
				else {
					tmp.setFinish(false);
				}
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
	if ((SDL_GetTicks() - jumpTimer) >= MAX_JUMPTIME && jumpCharge) {
		if (keystate[SDL_SCANCODE_RIGHT]) {
			velDX = velAir;
		}
		else if (keystate[SDL_SCANCODE_LEFT]) {
			velDX = -velAir;
		}
		velDY = -velY * (MAX_JUMPTIME*2 * 0.0015);
		jumpCharge = false;
		Mix_PlayChannel(-1, player.getSoundJump(), 0);
	}
	if (air) {
		flPrevTime = flCurTime;
		flCurTime = SDL_GetTicks();
		dt = (flCurTime - flPrevTime) * 0.001;
		if (dt >= 0.015) {
			dt = 0.015;
		}

		if (velDY < gravity) {
			velDY = velDY + gravity * dt;
		}
		else {
			velDY = gravity;
		}
		
		if (player.getCurAnimation() % 2 == 0) {
			player.setCurAnimation(jumpL);
		}
		else {
			player.setCurAnimation(jumpR);
		}
	}
	else {
		if (player.getCurAnimation()== jumpL) {
			player.setCurAnimation(idolL);
		}
		else if (player.getCurAnimation() == jumpR) {
			player.setCurAnimation(idolR);
		}
	}

	player.move(velDX, 0);
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1 && map[i].getSolid()) {
			if (map[i].getFinish()) {
				isRunning = false;
			}
			player.move(-velDX, 0);
			if (air) {
				velDX = velDX * -1;
				Mix_PlayChannel(-1, player.getSoundCol(), 0);
			}
		}
	}
	player.move(0, velDY);
	for (int i = 0; i < map.size(); i++) {
		if (checkCollision(player, map[i]) == 1 && map[i].getSolid()) {
			player.move(0, -velDY);
			if (velDY < 0) {
				velDY = velDY * -1;
				Mix_PlayChannel(-1, player.getSoundCol(), 0);
			}
			else {
				groundCol = true;
			}
		}
	}
	if (groundCol) {
		player.move(1, 0);
		for (int i = 0; i < map.size(); i++) {
			if (checkCollision(player, map[i]) == 1 && map[i].getSolid()) {
				player.move(1, 0);
			}
		}
		while (!colision) {
			player.move(0, 1);
			for (int i = 0; i < map.size(); i++) {
				if (checkCollision(player, map[i]) == 1 && map[i].getSolid()) {
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
		if (checkCollision(player, map[i]) == 1 && map[i].getSolid() && map[i].getSolid()) {
			colision = true;
		}
	}
	if (colision) {
		colision = false;
		if (air) {
			velDX = 0;
			velDY = 0;
			air = false;
			Mix_PlayChannel(-1, player.getSoundCol(), 0);
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
	SDL_DestroyTexture(player.getTex());
	player.destroySounds();
	player.destroy();
	for (int i = 0; i < map.size(); i++) {
		SDL_DestroyTexture(map[i].getTex());
		map[i].destroy();
	}
	keystate = nullptr;
	Mix_FreeMusic(bgm);
	window = nullptr;
	renderer = nullptr;
	bgm = nullptr;
	Mix_Quit();
	SDL_Quit();

}