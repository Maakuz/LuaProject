#include "GameOverHandler.h"

GameOverHandler::GameOverHandler(sf::Sprite* dummy, AudioHandler* audioHandler, Player** player, sf::RenderWindow* window)
{

	if (!this->starTex.loadFromFile("../Res/Tex/Items/star.png"))
		exit(-67);

	this->dummy = dummy;
	this->audioHandler = audioHandler;
	this->player = player;
	this->window = window;

	this->gameOverDummyColor = sf::Color(0, 0, 0, 0);
	this->starCounter = 0;
	this->starTimer = 0.5f;
}

GameOverHandler::~GameOverHandler()
{
}

void GameOverHandler::update(float dt)
{
	if (!this->audioHandler->isDeathPlaying())
	{
		sf::Vector2f dir = (*this->player)->getPos() - this->playerView.getCenter();

		this->playerView.setCenter(this->playerView.getCenter() + (0.1f * dt * dir));

		//16:10 ratio
		sf::Vector2f minSize(160, 100);
		dir = minSize - this->playerView.getSize();

		this->playerView.setSize(this->playerView.getSize() + (0.1f * dt * dir));

		//Timestampsaker
		if (this->audioHandler->getGameOverPos() > 35700)
		{
			if (gameOverDummyColor.a > 0)
				gameOverDummyColor.a -= 1;

			this->backView.setCenter(this->backView.getCenter().x + 1, this->backView.getCenter().y);

			this->starCounter += dt * ((rand() % 100) / 100.f);
			if (this->starCounter > this->starTimer)
			{
				starCounter = 0;
				this->stars.insertAt(this->stars.size(),
					AnimatedEntity(&this->starTex,
						this->starTex.getSize().x / 2,
						this->starTex.getSize().y,
						sf::Vector2i(2, 1), 10));

				//Get a random position from the viewports top to bottom
				float yPos = rand() % (int)this->backView.getSize().y +
					(this->backView.getCenter().y -
					(this->backView.getSize().y / 2));

				this->stars[this->stars.size() - 1].setPosition(this->backView.getCenter().x + 30 + (this->backView.getSize().x / 2), yPos);
			}

		}

		else if (gameOverDummyColor.a < 255)
			gameOverDummyColor.a += 1;

		this->dummy->setColor(this->gameOverDummyColor);


		for (int i = 0; i < this->stars.size(); i++)
		{
			this->stars[i].animate();
		}


		if (!this->audioHandler->playingGameOver())
			this->window->close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		this->window->close();
}

void GameOverHandler::setViews(sf::View view)
{
	this->backView = view;
	this->playerView = view;
}

void GameOverHandler::swapToBackView()
{
	if (this->audioHandler->getGameOverPos() > 35700)
	{
		this->window->setView(this->backView);
	}
}

void GameOverHandler::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	window->setView(this->backView);
	for (int i = 0; i < this->stars.size(); i++)
	{
		target.draw(this->stars[i]);
	}


	window->setView(this->playerView);
	target.draw(**this->player);
}
