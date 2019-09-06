#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "SFML/Graphics.hpp"
#include <sstream>

enum GameState { playing, paused, levelWon, dead, gameOver, gameWon };


class UserInterface : public sf::Drawable
{
private:
	int nrOfFish;
	int nrOfLives;

	sf::Sprite fish;
	sf::Sprite life;

	sf::Font font;

	sf::Text fishText;
	sf::Text lifeText;

	sf::Vector2f pauseTextPos;
	sf::Vector2f pauseTextOffset;
	sf::Text pauseText;
	sf::Text pauseTextBG;

	GameState state;

public:
	UserInterface(sf::Texture* fishTexture, sf::Texture* lifeTexture);
	virtual ~UserInterface();

	void update(float dt);

	void addLife(int amount = 1);
	void addFish(int amount = 1);
	int getFish() const;
	int getLives() const;

	GameState getGameState() const;
	void setGameState(GameState state);


	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


#endif // !USERINTERFACE_H
