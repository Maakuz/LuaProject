#include "UserInterface.h"

UserInterface::UserInterface(sf::Texture* fishTexture, sf::Texture* lifeTexture)
{
	this->fish.setTexture(*fishTexture);
	this->life.setTexture(*lifeTexture);
	this->life.setPosition(1190, 10);
	this->fish.setPosition(1190, 40);

	this->font.loadFromFile("../Res/PixelOperator8.ttf");
	this->fishText.setFont(this->font);
	this->lifeText.setFont(this->font);
	this->pauseText.setFont(this->font);
	this->pauseTextBG.setFont(this->font);


	this->lifeText.setPosition(1230, 10);
	this->fishText.setPosition(1230, 40);
	this->lifeText.setString("x 3");
	this->fishText.setString("x 0");
	this->lifeText.setCharacterSize(14);
	this->fishText.setCharacterSize(14);

	this->pauseTextPos = sf::Vector2f(240, 200);
	this->pauseTextOffset = sf::Vector2f(4, 4);

	this->pauseText.setPosition(this->pauseTextPos);
	this->pauseText.setString("PAUSED");
	this->pauseText.setCharacterSize(144);

	this->pauseTextBG.setPosition(this->pauseTextPos + pauseTextOffset);
	this->pauseTextBG.setString("PAUSED");
	this->pauseTextBG.setCharacterSize(144);
	this->pauseTextBG.setFillColor(sf::Color::Black);

	this->nrOfFish = 0;
	this->nrOfLives = 3;


}

UserInterface::~UserInterface()
{
	
}

void UserInterface::update(float dt)
{
	
}

void UserInterface::addLife(int amount)
{
	this->nrOfLives += amount;
	std::stringstream temp;
	temp << "x " << this->nrOfLives;

	this->lifeText.setString(temp.str());
}

void UserInterface::addFish(int amount)
{
	this->nrOfFish += amount;

	std::stringstream temp;
	temp << "x " << this->nrOfFish;

	this->fishText.setString(temp.str());
}

int UserInterface::getFish() const
{
	return this->nrOfFish;
}

int UserInterface::getLives() const
{
	return this->nrOfLives;
}

GameState UserInterface::getGameState() const
{
	return this->state;
}

void UserInterface::setGameState(GameState state)
{
	this->state = state;
}

void UserInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	switch (this->state)
	{
	case GameState::paused:
		target.draw(this->fish);
		target.draw(this->life);

		target.draw(this->fishText);
		target.draw(this->lifeText);


		target.draw(this->pauseTextBG);
		target.draw(this->pauseText);

		break;

	case GameState::playing:
		target.draw(this->fish);
		target.draw(this->life);

		target.draw(this->fishText);
		target.draw(this->lifeText);
		break;

	default:
		break;
	}
}
