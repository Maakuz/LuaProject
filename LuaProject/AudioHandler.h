#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H
#include "SFML/Audio.hpp"
#include "ScriptLoader.h"

class AudioHandler 
{
private:
	sf::SoundBuffer jumpB;
	sf::SoundBuffer deathB;
	sf::SoundBuffer hitEnemyB;
	sf::SoundBuffer pickupFishB;
	sf::SoundBuffer pickupLifeB;

	sf::Sound jump;
	sf::Sound death;
	sf::Sound hitEnemy;
	sf::Sound pickupFish;
	sf::Sound pickupLife;

	sf::Music level[3];
	sf::Music gameOver;

	lua_State* state;

	bool playing;
	bool gameOverStarted;

	void playJumpSfx(int volume);
	void playDeathSfx(int volume);
	void playHitSoundSfx(int volume);
	void playPickupFish(int volume);
	void playPickupLife(int volume);

	static int luaPlayJump(lua_State* state);
	static int luaPlayDead(lua_State* state);
	static int luaPlayHit(lua_State* state);
	static int luaPlayFish(lua_State* state);
	static int luaPlayLife(lua_State* state);

public:
	AudioHandler();
	virtual ~AudioHandler();


	void playMusic(int level);
	void stopMusic();
	bool playingGameOver();
	float getGameOverPos();

	bool isDeathPlaying() const;

	bool isPlaying() const;
	void toneDown(int level);
	void toneUp(int level);

	void callLuaFunc(char* path);

	void updateLua();

};

#endif