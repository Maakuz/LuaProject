#ifndef ANIMATEDENTITY
#define ANIMATEDENTITY
#include "MovingEntity.h"

class AnimatedEntity : public MovingEntity
{
private:
	sf::Vector2i sheetSize;
	sf::Vector2i currentFrame;
	sf::Vector2i frameSize;

	int animationCounter;
	int animationTime;

public:
	AnimatedEntity(sf::Texture* texture, int width, int height, sf::Vector2i sheetSize, int animTime = 100);

	void animate();
};


#endif