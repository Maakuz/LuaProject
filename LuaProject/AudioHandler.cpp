#include "AudioHandler.h"

AudioHandler::AudioHandler()
{
	this->playing = false;
	this->gameOverStarted = false;

	this->jumpB.loadFromFile("../Res/sfx/Jump.wav");
	this->deathB.loadFromFile("../Res/sfx/death.wav");
	this->hitEnemyB.loadFromFile("../Res/sfx/Hit.wav");
	this->pickupFishB.loadFromFile("../Res/sfx/fish.wav");
	this->pickupLifeB.loadFromFile("../Res/sfx/life.ogg");
	
	this->level[0].openFromFile("../Res/sfx/Level1.wav");
	this->level[1].openFromFile("../Res/sfx/Level2.wav");
	this->level[2].openFromFile("../Res/sfx/Level3.wav");
	this->gameOver.openFromFile("../Res/sfx/GameOver.wav");

	this->jump.setBuffer(this->jumpB);
	this->death.setBuffer(this->deathB);
	this->hitEnemy.setBuffer(this->hitEnemyB);
	this->pickupLife.setBuffer(this->pickupLifeB);
	this->pickupFish.setBuffer(this->pickupFishB);

	ScriptLoader loader;

	this->state = luaL_newstate();
	luaL_openlibs(this->state);

	lua_pushlightuserdata(this->state, this);
	lua_pushcclosure(this->state, AudioHandler::luaPlayJump, 1);
	lua_setglobal(this->state, "luaPlayJump");

	lua_pushlightuserdata(this->state, this);
	lua_pushcclosure(this->state, AudioHandler::luaPlayDead, 1);
	lua_setglobal(this->state, "luaPlayDead");

	lua_pushlightuserdata(this->state, this);
	lua_pushcclosure(this->state, AudioHandler::luaPlayHit, 1);
	lua_setglobal(this->state, "luaPlayHit");

	lua_pushlightuserdata(this->state, this);
	lua_pushcclosure(this->state, AudioHandler::luaPlayFish, 1);
	lua_setglobal(this->state, "luaPlayFish");

	lua_pushlightuserdata(this->state, this);
	lua_pushcclosure(this->state, AudioHandler::luaPlayLife, 1);
	lua_setglobal(this->state, "luaPlayLife");

	this->updateLua();

}

AudioHandler::~AudioHandler()
{
	lua_close(this->state);
}

void AudioHandler::playMusic(int level)
{
	this->level[level].play();
	this->playing = true;
}

void AudioHandler::stopMusic()
{
	for (int i = 0; i < 3; i++)
	{
		if (level[i].getStatus() == sf::Music::Status::Playing)
			level[i].stop();
	}

	this->playing = false;
}

//This also returns false when it's finished 
bool AudioHandler::playingGameOver()
{
	if (!this->gameOverStarted)
	{
		this->gameOver.play();
		this->gameOverStarted = true;
	}

	if (this->gameOverStarted && this->gameOver.getStatus() == sf::Music::Status::Stopped)
		return false;

	return true;

}

float AudioHandler::getGameOverPos()
{
	return this->gameOver.getPlayingOffset().asMilliseconds();
}

int AudioHandler::luaPlayJump(lua_State* state)
{
	AudioHandler* c = static_cast<AudioHandler*>(lua_touserdata(state, lua_upvalueindex(1)));
	
	c->playJumpSfx(lua_tonumber(state, -1));

	return 0;
}

int AudioHandler::luaPlayDead(lua_State* state)
{
	AudioHandler* c = static_cast<AudioHandler*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->playDeathSfx(lua_tonumber(state, -1));

	return 0;
}

int AudioHandler::luaPlayHit(lua_State* state)
{
	AudioHandler* c = static_cast<AudioHandler*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->playHitSoundSfx(lua_tonumber(state, -1));

	return 0;
}

int AudioHandler::luaPlayFish(lua_State* state)
{
	AudioHandler* c = static_cast<AudioHandler*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->playPickupFish(lua_tonumber(state, -1));

	return 0;
}

int AudioHandler::luaPlayLife(lua_State* state)
{
	AudioHandler* c = static_cast<AudioHandler*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->playPickupLife(lua_tonumber(state, -1));

	return 0;
}

void AudioHandler::playJumpSfx(int volume)
{
	this->jump.setVolume(volume);
	this->jump.play();
}

void AudioHandler::playDeathSfx(int volume)
{
	this->death.setVolume(volume);
	this->death.play();
}

void AudioHandler::playHitSoundSfx(int volume)
{
	this->hitEnemy.setVolume(volume);
	this->hitEnemy.play();
}

void AudioHandler::playPickupFish(int volume)
{
	this->pickupFish.setVolume(volume);
	this->pickupFish.play();
}

void AudioHandler::playPickupLife(int volume)
{
	this->pickupLife.setVolume(volume);
	this->pickupLife.play();
}

bool AudioHandler::isDeathPlaying() const
{
	return this->death.getStatus() == sf::Sound::Status::Playing;
}

bool AudioHandler::isPlaying() const
{
	return this->playing;
}

void AudioHandler::toneDown(int level)
{
	this->level[level].setVolume(this->level[level].getVolume()/4);
}

void AudioHandler::toneUp(int level)
{
	lua_getglobal(state, "musicVolume");

	this->level[0].setVolume(lua_tonumber(state, -1));

	lua_pop(state, 1);
}

void AudioHandler::callLuaFunc(char* path)
{
	lua_getglobal(this->state, path);

	lua_pcall(this->state, 0, 0, 0);
}

void AudioHandler::updateLua()
{
	ScriptLoader loader;

	if (!loader.loadScript("../Res/Scripts/AudioManager.lua", this->state))
		exit(-54);

	lua_getglobal(state, "musicVolume");

	this->level[0].setVolume(lua_tonumber(state, -1));

	lua_pop(state, 1);

	lua_getglobal(state, "gameOverVolume");
	this->gameOver.setVolume(lua_tonumber(state, -1));

	lua_pop(state, 1);
}