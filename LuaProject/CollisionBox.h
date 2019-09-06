#ifndef COLLISION_H
#define COLLISION_H

#include "SFML/Graphics.hpp"

class CollisionBox
{
private:
	sf::IntRect fullBox;
	sf::IntRect top;
	sf::IntRect bottom;
	sf::IntRect right;
	sf::IntRect left;

public:
	CollisionBox(int width = 64, int height = 64);
	virtual ~CollisionBox();

	void updatePosition(int x, int y, int offsetX = 0, int offsetY = 0);
	void updatePosition(sf::Vector2i pos, sf::Vector2i offset = sf::Vector2i(0, 0));

	sf::IntRect getTop() const;
	sf::IntRect getRect() const;
	sf::IntRect getLeft() const;
	sf::IntRect getRight() const;
	sf::IntRect getBottom() const;
};

#endif