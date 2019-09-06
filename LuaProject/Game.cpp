#include "Game.h"

void Game::initialize()
{
	if (this->player)
		delete this->player;

	this->player = new Player(&this->playerTex,
		this->playerTex.getSize().x / 4,
		this->playerTex.getSize().y / 2,
		sf::Vector2i(4, 2),
		5);

	this->player->setupLuaFunctions(this->playerState);
	this->player->setupLuaFunctions(this->playerState);
	this->player->setLuaState(this->playerState);

	this->player->setAudioHandler(audioHandler);

	for (int i = 0; i < this->enemyStates.size(); i++)
	{
		lua_close(this->enemyStates[i]);
	}

	if (this->goal)
	{
		delete this->goal;
		this->goal = nullptr;
	}

	for (int i = 0; i < NROFLAYERS; i++)
	{
		this->blocks[i] = ArrayList<Block>();
	}

	this->enemies = ArrayList<Enemy>();
	this->fishes = ArrayList<Entity>();
	this->lives = ArrayList<Entity>();
	this->enemyStates = ArrayList<lua_State*>();

	userInterface->setGameState(GameState::playing);
}

void Game::loadFiles()
{
	if (!this->dummyTex.loadFromFile("../Res/Tex/dummyFadeOut.png"))
		exit(-1);

	if (!this->playerTex.loadFromFile("../Res/Tex/Characters/kattny.png"))
		exit(-2);

	if (!this->blockTex.loadFromFile("../Res/Tex/TileMaps/grass.png"))
		exit(-3);

	if (!this->enemyTex.loadFromFile("../Res/Tex/Characters/SlushigSak.png"))
		exit(-4);

	if (!this->enemy2Tex.loadFromFile("../Res/Tex/Characters/AlienSak.png"))
		exit(-5);

	if (!this->backTex.loadFromFile("../Res/Tex/Backgrounds/Background.png"))
		exit(-6);

	if (!this->backTexDeath.loadFromFile("../Res/Tex/Backgrounds/BlackBackground.png"))
		exit(-66);

	if (!this->fishTex.loadFromFile("../Res/Tex/Items/fish.png"))
		exit(-66);

	if (!this->lifeTex.loadFromFile("../Res/Tex/Items/life.png"))
		exit(-66);


	this->playerState = luaL_newstate();
	luaL_openlibs(this->playerState);

	if (!this->loader.loadScript("../Res/Scripts/PlayerScript.lua", this->playerState))
		exit(-7);

	//Special case for c++ call in Lua
	this->levelLoader = luaL_newstate();
	luaL_openlibs(this->levelLoader);

	this->addCFunction(this->levelLoader, this->LuaGetRows, "getRows");

	if (!this->loader.loadScript("../Res/Scripts/Loader.lua", this->levelLoader))
		exit(-8);

	//should not really be able to go wrong
	lua_getglobal(this->levelLoader, "findLevels");

	lua_pcall(this->levelLoader, 0, 0, 0);


	this->updater = luaL_newstate();
	luaL_openlibs(this->updater);

	this->addCFunction(this->updater, this->LuaHandleInputs,			"handleInput");
	this->addCFunction(this->updater, this->LuaHandleColission,			"handleCollision");
	this->addCFunction(this->updater, this->LuaUpdateViewPort,			"updateViewPort");
	this->addCFunction(this->updater, this->LuaUpdateUI,				"updateUI");
	this->addCFunction(this->updater, this->LuaCheckAlive,				"checkAlive");
	this->addCFunction(this->updater, this->LuaGetLives,				"getLives");
	this->addCFunction(this->updater, this->LuaSetDummyColor,			"setDummyColor");
	this->addCFunction(this->updater, this->LuaWaitForInput,			"waitForInput");
	this->addCFunction(this->updater, this->LuaInitializeGameOver,		"initializeGameOver");
	this->addCFunction(this->updater, this->LuaInitializeGameWon,		"initializeGameWon");
	this->addCFunction(this->updater, this->LuaUpdateGameOverEvent,		"updateGameOverEvent");
	this->addCFunction(this->updater, this->LuaStopMusic,				"stopMusic");
	this->addCFunction(this->updater, this->LuaHitGoalPost,				"hitGoal");
	this->addCFunction(this->updater, this->LuaGetCurrentLevel,			"getCurrentLevel");
	this->addCFunction(this->updater, this->LuaLoadLevel,				"loadLevel");

	if (!this->loader.loadScript("../Res/Scripts/Update.lua", this->updater))
		exit(-10);

}

void Game::reloadScripts()
{
	//Reload
	if (!this->loader.loadScript("../Res/Scripts/UpdatePosition.lua", this->playerState))
	{
		exit(-6);
	}

	if (!this->loader.loadScript("../Res/Scripts/Loader.lua", this->levelLoader))
	{
		exit(-7);
	}

	this->audioHandler->updateLua();

	//should not really be able to go wrong
	lua_getglobal(this->levelLoader, "findLevels");

	lua_pcall(this->levelLoader, 0, 0, 0);
}

void Game::updateViewPort()
{
	if (abs(this->view.getCenter().y - this->player->getPos().y) > 85.f)
		this->view.setCenter(this->view.getCenter().x, (this->view.getCenter().y - int(this->view.getCenter().y - this->player->getPos().y) / 10.f));

	if (abs(this->view.getCenter().x - this->player->getPos().x) > 75.f)
		this->view.setCenter(int(this->view.getCenter().x - (this->view.getCenter().x - this->player->getPos().x) / 15.f), this->view.getCenter().y);



	if (this->view.getCenter().x < this->view.getSize().x / 2)
		this->view.setCenter(this->view.getSize().x / 2, this->view.getCenter().y);

	if (this->view.getCenter().y > this->view.getSize().y / 2)
		this->view.setCenter(this->view.getCenter().x, this->view.getSize().y / 2);

	if (this->view.getCenter().x > this->width)
		this->view.setCenter(this->width, this->view.getCenter().y);


	//PARRALAX
	float parralaxX = (this->view.getCenter().x / width);
	float parralaxY = (this->view.getCenter().y / (height + (this->view.getSize().y / 2)));
	this->backView.setCenter(
		(parralaxX *
		(this->backTex.getSize().x - this->backView.getSize().x)) + 
		this->backView.getSize().x / 2,

		160 +
		(parralaxY *
		(this->backTex.getSize().y - this->backView.getSize().y)) +
		this->backView.getSize().y / 2);
}

void Game::selectLevel()
{
	lua_getglobal(this->updater, "selectLevel");

	lua_pcall(this->updater, 0, 0, 0);
}

void Game::setLayer1(int x, int y, int offset, std::stringstream& stream)
{
	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			int type;

			stream >> type;

			if (type < TOTALBLOCKS && type >= 25 && type <= 49)
			{
				Block temp(&this->blockTex,
					i * BLOCKWIDTH,
					((j + offset) * BLOCKHEIGHT) + (BLOCKHEIGHT / 2),
					BLOCKWIDTH,
					BLOCKHEIGHT / 2,
					(type % TILEMAPX) * BLOCKWIDTH,
					(type / TILEMAPX) * BLOCKHEIGHT);

				this->blocks[0].insertAt(blocks[0].size(), temp);
			}

			else if (type < TOTALBLOCKS && type != 24 && type != 0)
			{
				Block temp(&this->blockTex,
					i * BLOCKWIDTH,
					(j + offset) * BLOCKHEIGHT,
					BLOCKWIDTH,
					BLOCKHEIGHT,
					(type % TILEMAPX) * BLOCKWIDTH,
					(type / TILEMAPX) * BLOCKHEIGHT);

				this->blocks[0].insertAt(blocks[0].size(), temp);
			}

			else if (type == 102)
			{
				Enemy temp(&this->enemy2Tex, i * BLOCKWIDTH,
					(j + offset) * BLOCKHEIGHT,
					this->enemy2Tex.getSize().x / 4,
					this->enemy2Tex.getSize().y / 2,
					sf::Vector2i(4, 2),
					5);

				this->enemies.insertAt(enemies.size(), temp);

				//Create a state for each enemy since they all needs their own closures
				lua_State* temp2 = luaL_newstate();
				luaL_openlibs(temp2);
				if (!this->loader.loadScript("../Res/Scripts/EnemyAI.lua", temp2))
					exit(-9);

				this->enemyStates.insertAt(enemyStates.size(), temp2);

				enemies[enemies.size() - 1].setupLuaFunctions(enemyStates[enemyStates.size() - 1]);
				enemies[enemies.size() - 1].setLuaState(enemyStates[enemyStates.size()-1]);
			}

			else if (type == 103)
			{
				Entity fish(&this->fishTex, this->fishTex.getSize().x,
					this->fishTex.getSize().y);

				fish.setPosition(i * BLOCKWIDTH,
					(j + offset) * BLOCKHEIGHT);

				this->fishes.insertAt(fishes.size(), fish);
			}

			else if (type == 104)
			{
				Entity life(&this->lifeTex, this->lifeTex.getSize().x,
					this->lifeTex.getSize().y);

				life.setPosition(i * BLOCKWIDTH,
					(j + offset) * BLOCKHEIGHT);

				this->lives.insertAt(lives.size(), life);
			}

			else if (type == 101)
			{
				Enemy temp(&this->enemyTex, i * BLOCKWIDTH,
					(j + offset) * BLOCKHEIGHT,
					this->enemyTex.getSize().x / 4,
					this->enemyTex.getSize().y / 2,
					sf::Vector2i(4, 2),
					5);

				this->enemies.insertAt(enemies.size(), temp);

				//Create a state for each enemy since they all needs their own closures
				lua_State* temp2 = luaL_newstate();
				luaL_openlibs(temp2);
				if (!this->loader.loadScript("../Res/Scripts/EnemyAI.lua", temp2))
					exit(-9);

				this->enemyStates.insertAt(enemyStates.size(), temp2);

				enemies[enemies.size() - 1].setupLuaFunctions(enemyStates[enemyStates.size() - 1]);
				enemies[enemies.size() - 1].setLuaState(enemyStates[enemyStates.size() - 1]);
			}

			else if (type == 105)
			{
				player->setPosition(float(i * BLOCKWIDTH),
					float((j + offset) * BLOCKHEIGHT));
			}

			else if (type == 24)
			{
				if (this->goal)
					delete this->goal;

				this->goal = new Block(&this->blockTex,
					i * BLOCKWIDTH,
					(j + offset) * BLOCKHEIGHT,
					BLOCKWIDTH,
					BLOCKHEIGHT,
					(type % TILEMAPX) * BLOCKWIDTH,
					(type / TILEMAPX) * BLOCKHEIGHT);
			}
		}
	}
}

void Game::setLayer2(int x, int y, int offset, std::stringstream & stream)
{
	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			int type;

			stream >> type;

			if (type < TOTALBLOCKS && type >= 25 && type <= 49)
			{
				Block temp(&this->blockTex,
					i * BLOCKWIDTH,
					((j + offset) * BLOCKHEIGHT) + (BLOCKHEIGHT / 2),
					BLOCKWIDTH,
					BLOCKHEIGHT / 2,
					(type % TILEMAPX) * BLOCKWIDTH,
					(type / TILEMAPX) * BLOCKHEIGHT);

				this->blocks[1].insertAt(blocks[1].size(), temp);
			}

			else if (type < TOTALBLOCKS && type != 24 && type != 0)
			{
				Block temp(&this->blockTex,
					i * BLOCKWIDTH,
					(j + offset) * BLOCKHEIGHT,
					BLOCKWIDTH,
					BLOCKHEIGHT,
					(type % TILEMAPX) * BLOCKWIDTH,
					(type / TILEMAPX) * BLOCKHEIGHT);

				this->blocks[1].insertAt(blocks[1].size(), temp);
			}
		}
	}
}

void Game::setOtherLayers(int x, int y, int offset, std::stringstream & stream, int whatLayer)
{
	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			int type;

			stream >> type;

			if (type < TOTALBLOCKS && type >= 25 && type <= 49)
			{
				Block temp(&this->blockTex,
					i * BLOCKWIDTH,
					((j + offset) * BLOCKHEIGHT) + (BLOCKHEIGHT / 2),
					BLOCKWIDTH,
					BLOCKHEIGHT / 2,
					(type % TILEMAPX) * BLOCKWIDTH,
					(type / TILEMAPX) * BLOCKHEIGHT);

				this->blocks[whatLayer].insertAt(blocks[whatLayer].size(), temp);
			}

			else if (type < TOTALBLOCKS && type != 24 && type != 0)
			{
				Block temp(&this->blockTex,
					i * BLOCKWIDTH,
					(j + offset) * BLOCKHEIGHT,
					BLOCKWIDTH,
					BLOCKHEIGHT,
					(type % TILEMAPX) * BLOCKWIDTH,
					(type / TILEMAPX) * BLOCKHEIGHT);

				this->blocks[whatLayer].insertAt(blocks[whatLayer].size(), temp);
			}
		}
	}
}

Game::Game(sf::RenderWindow* window)
{
	this->window = window;

	this->loadFiles();

	this->audioHandler = new AudioHandler();
	this->userInterface = new UserInterface(&this->fishTex, &this->lifeTex);
	this->gameOverHandler = new GameOverHandler(&this->dummy, this->audioHandler, &this->player, this->window);

	this->back.setTexture(this->backTex);
	this->back.setPosition(0, 0);

	this->backDeath.setTexture(this->backTexDeath);
	this->backDeath.setPosition(0, 0);

	this->dummy.setTexture(this->dummyTex);
	this->dummy.setScale(3, 3);
	this->dummy.setColor(sf::Color(0, 0, 0, 0));

	lua_getglobal(this->updater, "initialize");

	lua_pcall(this->updater, 0, 0, 0);

	this->view.setSize((sf::Vector2f)this->window->getSize());
	this->view.setCenter(this->player->getPos());

	this->backView.setSize((sf::Vector2f)this->window->getSize());
	this->backView.setCenter(float(this->window->getSize().x / 2), float(this->window->getSize().y / 2));




	window->setView(this->view);
}

Game::~Game()
{
	lua_close(this->playerState);
	lua_close(this->levelLoader);

	for (int i = 0; i < this->enemyStates.size(); i++)
	{
		lua_close(this->enemyStates[i]);
	}

	lua_close(this->updater);
	delete this->player;
	delete this->goal;
	delete this->audioHandler;
	delete this->userInterface;
	delete this->gameOverHandler;
}

void Game::update(float dt)
{
	lua_getglobal(this->updater, "update");
	lua_pushnumber(this->updater, dt);
	lua_pushnumber(this->updater, this->userInterface->getGameState());

	lua_pcall(this->updater, 2, 0, 0);
}

void Game::objectCollision()
{
	if (this->player->collisionObject(this->fishes))
	{
		this->audioHandler->callLuaFunc("playFish");
		this->userInterface->addFish(1);

		if (this->userInterface->getFish() == 100)
		{
			this->audioHandler->callLuaFunc("playLife");
			this->userInterface->addLife(1);
			this->userInterface->addFish(-100);
		}
	}

	if (this->player->collisionObject(this->lives))
	{
		this->audioHandler->callLuaFunc("playLife");
		this->userInterface->addLife(1);
	}
}

void Game::loadLevel(std::string fileName)
{
	int x;
	int y;
	std::string layers[NROFLAYERS];
	std::string tilemapPath = "";

	initialize();

	lua_getglobal(this->levelLoader, "setFileName");
	lua_pushstring(this->levelLoader, fileName.c_str());

	lua_pcall(this->levelLoader, 1, 0, 0);


	lua_getglobal(this->levelLoader, "getDimensions");

	int error = lua_pcall(this->levelLoader, 0, 2, 0);

	if (error)
	{
		lua_pop(this->levelLoader, 1);

		std::cout << "Map not loaded, manually ";
		this->selectLevel();
	}

	else
	{
		x = (int)lua_tonumber(this->levelLoader, -2);
		y = (int)lua_tonumber(this->levelLoader, -1);

		this->width = x * BLOCKWIDTH;
		this->height = y * BLOCKHEIGHT;

		this->nrOfRows = y;

		lua_pop(this->levelLoader, 2);

		//get what tilemap to use
		lua_getglobal(this->levelLoader, "getTileMap");
		lua_pcall(this->levelLoader, 0, 1, 0);

		tilemapPath = lua_tostring(this->levelLoader, -1);

		lua_pop(this->levelLoader, 1);

		if (!this->blockTex.loadFromFile("../Res/Tex/TileMaps/" + tilemapPath))
		{
			if (!this->blockTex.loadFromFile("../Res/Tex/TileMaps/grass.png"))
			{
				exit(-3);
			}
		}

		//Get the map
		lua_getglobal(this->levelLoader, "getLevelData");

		lua_pushnumber(this->levelLoader, y);
		lua_pcall(this->levelLoader, 1, 5, 0);

		layers[0] = lua_tostring(this->levelLoader, -5);
		layers[1] = lua_tostring(this->levelLoader, -4);
		layers[2] = lua_tostring(this->levelLoader, -3);
		layers[3] = lua_tostring(this->levelLoader, -2);
		layers[4] = lua_tostring(this->levelLoader, -1);
		lua_pop(this->levelLoader, NROFLAYERS);

		int offset = (window->getSize().y / 32) - y;

		std::stringstream stream;
		stream << layers[0];

		this->setLayer1(x, y, offset, stream);

		stream = std::stringstream();
		stream << layers[1];

		this->setLayer2(x, y, offset, stream);

		for (int i = 2; i < NROFLAYERS; i++)
		{
			stream = std::stringstream();
			stream << layers[i];

			this->setOtherLayers(x, y, offset, stream, i);
		}

		window->requestFocus();
		this->audioHandler->playMusic(0);
	}
}

int Game::getRows(lua_State* state)
{
	lua_pushnumber(state, this->nrOfRows);
	return 1;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	window->setView(this->backView);


	if (userInterface->getGameState() == GameState::playing)
		target.draw(this->back);


	else if (userInterface->getGameState() == GameState::paused)
		target.draw(this->back);


	else if (userInterface->getGameState() == GameState::dead)
		target.draw(this->backDeath);


	window->setView(this->view);

	//Timestampsaker
	this->gameOverHandler->swapToBackView();


	for (int i = NROFLAYERS - 2; i >= 0; i--)
	{
		for (int j = 0; j < blocks[i].size(); j++)
		{
			target.draw(this->blocks[i][j]);
		}
	}

	for (int i = 0; i < this->fishes.size(); i++)
	{
		target.draw(this->fishes[i]);
	}

	for (int i = 0; i < this->lives.size(); i++)
	{
		target.draw(this->lives[i]);
	}

	for (int i = 0; i < this->enemies.size(); i++)
	{
		target.draw(this->enemies[i]);

	}

	if (userInterface->getGameState() != GameState::gameOver)
		target.draw(*this->player);


	//Last layer is above player
	for (int i = 0; i < blocks[NROFLAYERS - 1].size(); i++)
	{
		target.draw(this->blocks[NROFLAYERS - 1][i]);
	}

	if (this->goal)
		target.draw(*this->goal);

	this->window->setView(this->window->getDefaultView());

	if (this->userInterface->getGameState() != GameState::playing)
		target.draw(this->dummy);


	if (this->userInterface->getGameState() == GameState::gameOver)
		target.draw(*this->gameOverHandler);

	else if (this->userInterface->getGameState() == GameState::gameWon)
		target.draw(*this->gameOverHandler);


	target.draw(*this->userInterface);
}

void Game::addCFunction(lua_State* state, lua_CFunction func, char* funcName)
{
	lua_pushlightuserdata(state, this);
	lua_pushcclosure(state, func, 1);
	lua_setglobal(state, funcName);
}

//**************************************************************************************
//************************************LUA FUNCTIONS*************************************
//**************************************************************************************
int Game::LuaGetRows(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->getRows(state);

	return 1;
}

int Game::LuaHandleInputs(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
		c->reloadScripts();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F8))
		c->selectLevel();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
		c->window->close();

	//pause
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !c->wasEscapePressed)
	{
		if (c->userInterface->getGameState() == GameState::playing)
		{
			c->dummy.setColor(sf::Color(0, 0, 0, 50));

			c->userInterface->setGameState(GameState::paused);
			c->audioHandler->toneDown(0);
		}

		else if (c->userInterface->getGameState() == GameState::paused)
		{
			c->userInterface->setGameState(GameState::playing);
			c->audioHandler->toneUp(0);
		}
	}

	c->wasEscapePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
	//pauseEnd
	return 0;
}

int Game::LuaHandleColission(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	float deltaTime = lua_tonumber(state, -1);
	lua_pop(state, 1);

	//Collision detection
	//Relevant order. DO NOT CHANGE
	c->player->collisionEnemy(c->enemies);

	c->objectCollision();

	c->player->updatePosition(deltaTime);
	c->player->collisionWorldSides();
	c->player->collision(c->blocks[0]);
	c->player->collision(c->blocks[1]);
	c->player->collisionBottom(c->blocks[0]);
	c->player->collisionSides(c->blocks[0]);
	c->player->animate();


	for (int i = 0; i < c->enemies.size(); i++)
	{
		c->enemies[i].updatePosition(deltaTime);
		c->enemies[i].collision(c->blocks[0]);
		c->enemies[i].collision(c->blocks[1]);
		if (c->enemies[i].collisionSides(c->blocks[0]))
			c->enemies[i].setDirection(c->enemies[i].getDirection() * -1);
		c->enemies[i].animate();
	}

	return 0;
}

int Game::LuaUpdateViewPort(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));
	c->updateViewPort();

	return 0;
}

int Game::LuaUpdateUI(lua_State * state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	float deltaTime = lua_tonumber(state, -1);
	lua_pop(state, 1);

	c->userInterface->update(deltaTime);

	return 0;
}

int Game::LuaCheckAlive(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	//If the player dies
	if (!c->player->isAlive())
	{
		c->audioHandler->stopMusic();
		c->audioHandler->callLuaFunc("playDeathSound");
		c->userInterface->setGameState(GameState::dead);
		c->userInterface->addLife(-1);
	}

	return 0;
}

int Game::LuaGetLives(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));
	lua_pushnumber(state, c->userInterface->getLives());

	return 1;
}

int Game::LuaSetDummyColor(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	float r = lua_tonumber(state, -4);
	float g = lua_tonumber(state, -3);
	float b = lua_tonumber(state, -2);
	float a = lua_tonumber(state, -1);

	lua_pop(state, 4);

	c->dummy.setColor(sf::Color(r, g, b, a));

	return 0;
}

int Game::LuaWaitForInput(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	lua_pushboolean(state, (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)));

	return 1;
}

int Game::LuaInitializeGameOver(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));
	
	c->userInterface->setGameState(GameState::gameOver);
	c->gameOverHandler->setViews(c->view);

	return 0;
}

int Game::LuaInitializeGameWon(lua_State * state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->userInterface->setGameState(GameState::gameWon);
	c->gameOverHandler->setViews(c->view);

	return 0;
}

int Game::LuaUpdateGameOverEvent(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	float deltaTime = lua_tonumber(state, -1);
	lua_pop(state, 1);

	c->gameOverHandler->update(deltaTime);
	return 0;
}

int Game::LuaStopMusic(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->audioHandler->stopMusic();
	return 0;
}

int Game::LuaHitGoalPost(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	bool test = c->goal && c->player->getBox()->getRect().intersects(c->goal->getBox()->getRect());

	lua_pushboolean(state, test);

	return 1;
}

//Not my proudest moment
int Game::LuaGetCurrentLevel(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));

	lua_getglobal(state, "whatLevel");

	int whatLevel = lua_tointeger(c->updater, -1);
	lua_pop(c->updater, 1);

	lua_getglobal(c->levelLoader, "getFileName");
	lua_pushinteger(c->levelLoader, whatLevel);

	lua_pcall(c->levelLoader, 1, 1, 0);

	lua_pushstring(state, lua_tostring(c->levelLoader, -1));

	lua_pop(c->levelLoader, 1);

	return 1;
}

int Game::LuaLoadLevel(lua_State* state)
{
	Game* c = static_cast<Game*>(lua_touserdata(state, lua_upvalueindex(1)));
	
	std::string levelName = lua_tostring(state, -1);
	lua_pop(state, 1);

	c->loadLevel(levelName);

	return 0;
}

