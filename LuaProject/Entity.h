#ifndef ENTITY_H
#define ENTITY_H

#include "SFML/Graphics.hpp"
#include "CollisionBox.h"

class Entity : public sf::Drawable
{
private:
	sf::Texture* texture;
	sf::Sprite sprite;
	sf::Vector2f pos;

	CollisionBox box;

public:
	Entity(sf::Texture* tex, int width, int height);
	virtual ~Entity();

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position);

	sf::Vector2f getPos() const;
	CollisionBox* getBox();

	void setTextureRect(sf::IntRect rext);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif