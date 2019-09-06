#include "Block.h"

Block::Block(sf::Texture* tex, int x, int y, int width, int height, int u, int v)
: Entity(tex, width, height)
{
	this->setPosition(float(x), float(y));
	this->getBox()->updatePosition(x, y);
	this->setTextureRect(sf::IntRect(u, v, width, height));
}

Block::~Block()
{
}

