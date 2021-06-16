#pragma once
#include "Object.h"

class Player {
private: 
	float velocityX, velocityY, posX, posY;

public:
	Object player;
	void move(float mX, float mY);
	void setVelocity(float velX, float velY);
	
};