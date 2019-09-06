#ifndef PLAYER_H
#define PLAYER_H

#include "Enemy.h"
#include "AudioHandler.h"

class Player : public AnimatedEntity
{
private:

	sf::Keyboard ks;
	bool alive;
	
	AudioHandler* audioHandler;

	static int LuaHandleInputs(lua_State* state);

public:
	Player(sf::Texture* tex, int width, int height, sf::Vector2i sheetSize, int animTime = 100);
	virtual ~Player();

	void setAudioHandler(AudioHandler* audioHandler);
	void setupLuaFunctions(lua_State* state);

	void collisionEnemy(ArrayList<Enemy>& enemies);
	void collisionWorldSides();
	bool collisionObject(ArrayList<Entity>& objects);
	void move();

	bool isAlive() const;



};

#endif