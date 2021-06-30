#pragma once
#include "Object.h"

#include <vector>
class Player : public Object {
private: 
	struct animation {
		int row, w, h, spriteAmount, cycleSpeed, curTex;
	};
	std::vector<animation> animations;
	int curAni;
	int begin;

public:
	int createCycle(int row, int w, int h, int spriteAmount, int cycleSpeed);
	void setCurAnimation(int current) { curAni = current; begin = 0; };
	void updateAnimation();
	void jumpAnimation();
	int getCurAnimation() { return curAni; };

	
};