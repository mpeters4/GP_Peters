#include "Player.h"



int Player::createCycle(int row, int w, int h, int spriteAmount, int cycleSpeed) {
	animation tmp;
	tmp.row = row;
	tmp.w = w;
	tmp.h = h;
	tmp.spriteAmount = spriteAmount;
	tmp.cycleSpeed = cycleSpeed;
	tmp.curTex = 0;
	animations.push_back(tmp);
	return animations.size()-1;
}

void Player::updateAnimation() {
	if (animations[curAni].spriteAmount > 1) {
		//std::cout << begin << std::endl;
		setSrc(animations[curAni].w * animations[curAni].curTex, animations[curAni].row * animations[curAni].h, animations[curAni].w, animations[curAni].h);
		if (begin > animations[curAni].cycleSpeed) {
			animations[curAni].curTex++;
			begin = 0;
		}
		begin++;
		if (animations[curAni].curTex >= animations[curAni].spriteAmount) {
			animations[curAni].curTex = 0;
		}
	}
	else {
		setSrc(animations[curAni].w * animations[curAni].curTex, animations[curAni].row * animations[curAni].h, animations[curAni].w, animations[curAni].h);
	}
}

void Player::setSounds(Mix_Chunk* soundJump, Mix_Chunk* soundStep, Mix_Chunk* soundCol) {
	Player::soundJump = soundJump;
	Player::soundStep = soundStep;
	Player::soundCol = soundCol;
}

void Player::destroySounds() {
	Mix_FreeChunk(soundCol);
	Mix_FreeChunk(soundJump);
	Mix_FreeChunk(soundStep);
}