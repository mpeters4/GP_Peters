#pragma once
#include "Object.h"
#include "SDL_mixer.h"


#include <vector>
class Player : public Object {
private: 
	struct animation {
		int row, w, h, spriteAmount, cycleSpeed, curTex;
	};
	Mix_Chunk* soundJump;
	Mix_Chunk* soundStep;
	Mix_Chunk* soundCol;
	std::vector<animation> animations;
	int curAni;
	int begin;

public:
	int createCycle(int row, int w, int h, int spriteAmount, int cycleSpeed);
	void setCurAnimation(int current) { curAni = current; begin = 0; };
	void updateAnimation();
	void setSounds(Mix_Chunk* soundJump, Mix_Chunk* soundStep, Mix_Chunk* soundCol);
	int getCurAnimation() { return curAni; };
	Mix_Chunk* getSoundJump() { return soundJump; };
	Mix_Chunk* getSoundStep() { return soundStep; };
	Mix_Chunk* getSoundCol() { return soundCol; };
	void destroySounds();

	
};