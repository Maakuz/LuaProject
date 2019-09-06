#ifndef GAMEOVERHANDLER_H
#define GAMEOVERHANDLER_H
#include "SFML/Graphics.hpp"
#include "ArrayList.h"
#include "AnimatedEntity.h"
#include "AudioHandler.h"
#include "Player.h"


class GameOverHandler : public sf::Drawable
{
private:
	sf::Texture starTex;

	sf::Color gameOverDummyColor;

	ArrayList<AnimatedEntity> stars;
	float starTimer;
	float starCounter;

	sf::Sprite* dummy; 
	Player** player;

	AudioHandler* audioHandler;
	sf::View playerView;
	sf::View backView;
	sf::RenderWindow* window;

public:
	GameOverHandler(sf::Sprite* dummy, AudioHandler* audioHandler, Player** player, sf::RenderWindow* window);
	virtual ~GameOverHandler();

	void update(float dt);

	void setViews(sf::View view);

	void swapToBackView();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};


#endif
