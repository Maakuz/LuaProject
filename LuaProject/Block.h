#ifndef BLOCK_H
#define BLOCK_H
#include "Entity.h"
#include "CollisionBox.h"

class Block : public Entity
{
private:

public:
	Block(sf::Texture* tex, int x, int y, int width, int height, int u, int v);
	virtual ~Block();

	
};



#endif