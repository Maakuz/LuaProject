#include "CollisionBox.h"

CollisionBox::CollisionBox(int width, int height)
{
	this->fullBox.left = 0;
	this->fullBox.top = 0;

	this->fullBox.width = width;
	this->fullBox.height = height;

	this->top.top = 0;
	this->top.left = 0;

	this->top.width = width;
	this->top.height = (height / 2);

	this->bottom.top = 0;
	this->bottom.left = 0;

	this->bottom.width = width - (width / 4);
	this->bottom.height = (height / 2);

	this->left.left = 0;
	this->left.top = 0;

	this->left.width = (width / 2);
	this->left.height = height;

	this->right.left = 0;
	this->right.top = 0;

	this->right.width = (width / 2);
	this->right.height = height;
}

CollisionBox::~CollisionBox()
{
}

void CollisionBox::updatePosition(int x, int y, int offsetX, int offsetY)
{
	this->fullBox.left = x + offsetX;
	this->fullBox.top = y + offsetY;

	this->top.top = y + offsetY;
	this->top.left = x + offsetX;

	this->bottom.top = y + offsetY + fullBox.height - this->bottom.height;
	this->bottom.left = x + offsetX + (this->bottom.width / 8);

	this->left.top = y + offsetY;
	this->left.left = x + offsetX;

	this->right.top = y + offsetY;
	this->right.left = x + offsetX + fullBox.width - this->right.width;
}

void CollisionBox::updatePosition(sf::Vector2i pos, sf::Vector2i offset)
{
	this->fullBox.left = pos.x + offset.x;
	this->fullBox.top = pos.y + offset.y;

	this->top.top = pos.y + offset.y;
	this->top.left = pos.x + offset.x;

	this->bottom.top = pos.y + offset.y + fullBox.height - this->bottom.height;
	this->bottom.left = pos.x + offset.x + (this->bottom.width / 8);

	this->left.top = pos.y + offset.y;
	this->left.left = pos.x + offset.x;

	this->right.top = pos.y + offset.y;
	this->right.left = pos.x + offset.x + fullBox.width - this->right.width;
}

sf::IntRect CollisionBox::getTop() const
{
	return this->top;
}

sf::IntRect CollisionBox::getRect() const
{
	return this->fullBox;
}

sf::IntRect CollisionBox::getLeft() const
{
	return this->left;
}

sf::IntRect CollisionBox::getRight() const
{
	return this->right;
}
sf::IntRect CollisionBox::getBottom() const
{
	return this->bottom;
}
