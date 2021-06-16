#include "Player.h"

void Player::move(float mX, float mY) {
	posX += velocityX * mX;
	posY += velocityY * mY;
	player.setPos(posX, posY);
}	

void Player::setVelocity(float velX, float velY) {
	
}