#ifndef ENEMY_H
#define ENEMY_H

#include "AnimatedEntity.h"

class Enemy : public AnimatedEntity
{
private:

public:
	Enemy(sf::Texture* tex, int x, int y, int width, int height, sf::Vector2i sheetSize, int animTime = 100);
	virtual ~Enemy();
};

#endif