#include "Entity.h"

Entity::Entity(sf::Texture* tex, int width, int height)
{
	this->texture = tex;
	this->sprite.setTexture(*texture);
	this->box = CollisionBox(width, height);
}

Entity::~Entity()
{
}

void Entity::setPosition(float x, float y)
{
	this->pos.x = x;
	this->pos.y = y;
	this->sprite.setPosition((sf::Vector2f)(sf::Vector2i)this->pos);
	this->box.updatePosition((sf::Vector2i)this->pos);
}

void Entity::setPosition(sf::Vector2f position)
{
	this->pos = position;

	//My greatest gift to the world.
	this->sprite.setPosition((sf::Vector2f)(sf::Vector2i)this->pos);
	this->box.updatePosition((sf::Vector2i)this->pos);
}

sf::Vector2f Entity::getPos() const
{
	return this->pos;
}

CollisionBox* Entity::getBox()
{
	return &this->box;
}

void Entity::setTextureRect(sf::IntRect rext)
{
	this->sprite.setTextureRect(rext);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->sprite);
}