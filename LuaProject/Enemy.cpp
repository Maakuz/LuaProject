#include "Enemy.h"

Enemy::Enemy(sf::Texture* texture, int x, int y, int width, int height, sf::Vector2i sheetSize, int animTime)
	: AnimatedEntity(texture, width, height, sheetSize, animTime)
{
	this->setPosition(float(x), (float)y);
	this->getBox()->updatePosition(x, y);
}

Enemy::~Enemy()
{
}