#ifndef MOVINGENTITY_H
#define MOVINGENTITY_H

#include "Entity.h"
#include "ArrayList.h"
#include "ScriptLoader.h"
#include <iostream>
#include "Block.h"

class MovingEntity : public Entity
{
private:
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	lua_State* script;

	int dir;
	bool isJumping;
	bool isMoving;

	static int LuaGetDir(lua_State* state);
	static int LuaGetAcceleration(lua_State* state);
	static int LuaSetAcceleration(lua_State* state);
	static int LuaSetVelocity(lua_State* state);
	static int LuaGetVelocity(lua_State* state);
	static int LuaSetPosition(lua_State* state);

protected:
	void addCFunction(lua_State* state, lua_CFunction func, char* funcName, void* p);

public:
	MovingEntity(sf::Texture* texture, int width, int height);
	virtual ~MovingEntity();

	virtual void setupLuaFunctions(lua_State* state);

	void collision(ArrayList<Block>& blocks);
	bool collisionSides(ArrayList<Block>& blocks);
	bool collisionBottom(ArrayList<Block>& blocks);

	//to remove 
	void LuaMove(int dir);
	void updateGravity();

	void updatePosition(float dt);

	void setLuaState(lua_State* state);
	lua_State* getLuaState();
	void jump(float height);

	bool getIsJumping() const;
	void setIsJumping(bool value);

	float getJumpHeight() const;

	//stops all form of movement
	void zeroMovement();

	void setAcceleration(sf::Vector2f acceleration);
	void setAcceleration(float x, float y);
	sf::Vector2f getAcceleration() const;

	sf::Vector2f getVelocity() const;
	void setVelocity(float x, float y);
	void setVelocity(sf::Vector2f velocity);
	
	int getDirection() const;
	void setDirection(int dir);

	void setMoving(bool value);
	bool getMoving() const;
};

#endif
