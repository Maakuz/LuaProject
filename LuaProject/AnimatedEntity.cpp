#include "AnimatedEntity.h"

AnimatedEntity::AnimatedEntity(sf::Texture* texture, int width, int height, sf::Vector2i sheetSize, int animTime)
	:MovingEntity(texture, width, height)
{
	this->animationCounter = 0;
	this->animationTime = animTime;
	this->currentFrame = sf::Vector2i(0, 0);
	this->sheetSize = sheetSize;
	this->frameSize.x = texture->getSize().x / sheetSize.x;
	this->frameSize.y = texture->getSize().y / sheetSize.y;
	this->setTextureRect(sf::IntRect(0, 0, width, height));
}

void AnimatedEntity::animate()
{
	this->animationCounter++;

	if (this->animationCounter > this->animationTime)
	{
		this->animationCounter = 0;

		if (++this->currentFrame.x >= this->sheetSize.x)
		{
			this->currentFrame.x = 0;
		}
	}

	if (!this->getMoving())
		this->currentFrame.x = 0;

	if (this->getIsJumping())
		this->currentFrame.x = 1;

	if (this->getDirection() < 0)
		this->currentFrame.y = 1;

	else if (this->getDirection() > 0)
		this->currentFrame.y = 0;

	this->setTextureRect(sf::IntRect(
		this->currentFrame.x * this->frameSize.x, 
		this->currentFrame.y * this->frameSize.y,
		this->frameSize.x,
		frameSize.y));
}
