#ifndef GAME_H
#define GAME_H
#include "UserInterface.h"
#include "Block.h"
#include "GameOverHandler.h"
#include "Enemy.h"
#include <string>
#include <sstream>

#define BLOCKWIDTH 32
#define BLOCKHEIGHT 32
#define TILEMAPX 25
#define TILEMAPY 4
#define TOTALBLOCKS TILEMAPX * TILEMAPY
#define NROFLAYERS 5

class Game : public sf::Drawable
{
private:
	sf::Texture playerTex;
	sf::Texture enemyTex;
	sf::Texture enemy2Tex;
	sf::Texture blockTex;
	sf::Texture backTex;
	sf::Texture backTexDeath;
	sf::Texture lifeTex;
	sf::Texture fishTex;

	sf::Texture dummyTex;
	sf::Sprite dummy;

	sf::RenderWindow* window;
	sf::View view;
	sf::View backView;

	sf::Sprite back;
	sf::Sprite backDeath;
	ScriptLoader loader;

	lua_State* playerState;
	lua_State* levelLoader;
	lua_State* updater;
	ArrayList<lua_State*> enemyStates;

	int height;
	int width;


	Player* player;
	ArrayList<Block> blocks[NROFLAYERS];
	ArrayList<Enemy> enemies;

	ArrayList<Entity> fishes;
	ArrayList<Entity> lives;

	Block* goal;

	AudioHandler* audioHandler;
	UserInterface* userInterface;
	GameOverHandler* gameOverHandler;

	int nrOfRows;
	bool wasEscapePressed;


	void initialize();
	void loadFiles();
	void reloadScripts();
	void updateViewPort();
	void objectCollision();

	void selectLevel();

	void addCFunction(lua_State* state, lua_CFunction func, char* funcName);

	static int LuaGetRows(lua_State* state);
	static int LuaHandleInputs(lua_State* state);
	static int LuaHandleColission(lua_State* state);
	static int LuaUpdateViewPort(lua_State* state);
	static int LuaUpdateUI(lua_State* state);
	static int LuaCheckAlive(lua_State* state);
	static int LuaGetLives(lua_State* state);
	static int LuaSetDummyColor(lua_State* state);
	static int LuaWaitForInput(lua_State* state);
	static int LuaInitializeGameOver(lua_State* state);
	static int LuaInitializeGameWon(lua_State* state);
	static int LuaUpdateGameOverEvent(lua_State* state);
	static int LuaStopMusic(lua_State* state);
	static int LuaHitGoalPost(lua_State* state);
	static int LuaGetCurrentLevel(lua_State* state);
	static int LuaLoadLevel(lua_State* state);

	void setLayer1(int x, int y, int offset, std::stringstream& stream);
	void setLayer2(int x, int y, int offset, std::stringstream& stream);
	void setOtherLayers(int x, int y, int offset, std::stringstream& stream, int whatLayer);

public:
	Game(sf::RenderWindow* window);
	virtual ~Game();

	void update(float dt);

	void loadLevel(std::string fileName);

	int getRows(lua_State* state);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;


};

#endif