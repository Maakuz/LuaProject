#include "MovingEntity.h"


void MovingEntity::addCFunction(lua_State* state, lua_CFunction func, char* funcName, void* p)
{
	lua_pushlightuserdata(state, p);
	lua_pushcclosure(state, func, 1);
	lua_setglobal(state, funcName);
}

MovingEntity::MovingEntity(sf::Texture* texture, int width, int height)
	:Entity(texture, width, height)
{
	this->dir = 1;

	this->isJumping = false;
	this->isMoving = true;

	this->acceleration = sf::Vector2f(0, 0);
	this->velocity = sf::Vector2f(0, 0);

}

MovingEntity::~MovingEntity()
{
}

void MovingEntity::setupLuaFunctions(lua_State* state)
{
	this->addCFunction(state, this->LuaGetDir, "getDir", this);
	this->addCFunction(state, this->LuaGetAcceleration, "getAccel", this);
	this->addCFunction(state, this->LuaSetAcceleration, "setAccel", this);
	this->addCFunction(state, this->LuaSetVelocity, "setVelocity", this);
	this->addCFunction(state, this->LuaGetVelocity, "getVelocity", this);
	this->addCFunction(state, this->LuaSetPosition, "setPosition", this);
}

void MovingEntity::collision(ArrayList<Block>& blocks)
{
	bool exit = false;

	for (int i = 0; i < blocks.size() && !exit; i++)
	{
		if (this->velocity.y > 0 && blocks[i].getBox()->getTop().intersects(getBox()->getBottom()))
		{
			this->setPosition(this->getPos().x, blocks[i].getPos().y - this->getBox()->getRect().height);

			this->velocity.y = 0;
			this->acceleration.y = 0;
			this->isJumping = false;
			exit = true;
		}
	}
}

bool MovingEntity::collisionSides(ArrayList<Block>& blocks)
{
	bool res = false;
	for (int i = 0; i < blocks.size() && !res; i++)
	{
		if (blocks[i].getBox()->getLeft().intersects(getBox()->getRight()))
		{
			this->setPosition(blocks[i].getPos().x - this->getBox()->getRect().width, this->getPos().y);

			this->velocity.x = 0;
			this->acceleration.x = 0;
			res = true;
		}

		else if (blocks[i].getBox()->getRight().intersects(getBox()->getLeft()))
		{
			this->setPosition(blocks[i].getPos().x + blocks[i].getBox()->getRect().width, this->getPos().y);

			this->velocity.x = 0;
			this->acceleration.x = 0;
			res = true;
		}
	}

	return res;
}

bool MovingEntity::collisionBottom(ArrayList<Block>& blocks)
{
	bool exit = false;

	for (int i = 0; i < blocks.size() && !exit; i++)
	{
		if (this->velocity.y < 0 && blocks[i].getBox()->getBottom().intersects(getBox()->getTop()))
		{
			this->setPosition(this->getPos().x, blocks[i].getPos().y + blocks[i].getBox()->getRect().height);

			this->velocity.y = 0;
			this->acceleration.y = 0;
			exit = true;
		}
	}

	return exit;
}

void MovingEntity::updatePosition(float dt)
{
	lua_getglobal(this->script, "update");
	int test = lua_pcall(this->script, 0, 0, 0);
	if (test != 0)
	{
		std::cout << lua_tostring(this->script, -1) << std::endl;
		lua_pop(this->script, 1);
	}
}

void MovingEntity::setLuaState(lua_State* state)
{
	this->script = state;
}

lua_State* MovingEntity::getLuaState()
{
	return this->script;
}

void MovingEntity::jump(float height)
{
	this->isJumping = true;
	this->setAcceleration(sf::Vector2f(this->getAcceleration().x, height));
}

bool MovingEntity::getIsJumping() const
{
	return this->isJumping;
}

void MovingEntity::setIsJumping(bool value)
{
	this->isJumping = value;
}

float MovingEntity::getJumpHeight() const
{
	lua_getglobal(this->script, "jumpHeight");

	float val = (float)lua_tonumber(this->script, -1);

	lua_pop(this->script, 1);

	return -val;
}

void MovingEntity::zeroMovement()
{
	this->acceleration.x = 0;
	this->acceleration.y = 0;

	this->velocity.x = 0;
	this->velocity.y = 0;
}

void MovingEntity::setAcceleration(sf::Vector2f acceleration)
{
	this->acceleration = acceleration;
}

void MovingEntity::setAcceleration(float x, float y)
{
	this->acceleration.x = x;
	this->acceleration.y = y;
}

sf::Vector2f MovingEntity::getAcceleration() const
{
	return this->acceleration;
}

sf::Vector2f MovingEntity::getVelocity() const
{
	return this->velocity;
}

void MovingEntity::setVelocity(float x, float y)
{
	this->velocity.x = x;
	this->velocity.y = y;
}

void MovingEntity::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}

int MovingEntity::getDirection() const
{
	return this->dir;
}

void MovingEntity::setDirection(int dir)
{
	this->dir = dir;
}

void MovingEntity::setMoving(bool value)
{
	this->isMoving = value;
}

bool MovingEntity::getMoving() const
{
	return this->isMoving;
}

//***************************************************************
//*************************LUA FUNCTIONS*************************
//***************************************************************
int MovingEntity::LuaGetDir(lua_State* state)
{
	MovingEntity* c = static_cast<MovingEntity*>(lua_touserdata(state, lua_upvalueindex(1)));

	lua_pushnumber(state, c->dir);

	return 1;
}

int MovingEntity::LuaGetAcceleration(lua_State* state)
{
	MovingEntity* c = static_cast<MovingEntity*>(lua_touserdata(state, lua_upvalueindex(1)));

	lua_pushnumber(state, c->acceleration.x);
	lua_pushnumber(state, c->acceleration.y);

	return 2;
}

int MovingEntity::LuaSetAcceleration(lua_State * state)
{
	MovingEntity* c = static_cast<MovingEntity*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->acceleration.x = lua_tonumber(state, -2);
	c->acceleration.y = lua_tonumber(state, -1);

	lua_pop(state, 2);

	return 0;
}

int MovingEntity::LuaSetVelocity(lua_State* state)
{
	MovingEntity* c = static_cast<MovingEntity*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->velocity.x = lua_tonumber(state, -2);
	c->velocity.y = lua_tonumber(state, -1);

	lua_pop(state, 2);

	return 0;
}

int MovingEntity::LuaGetVelocity(lua_State* state)
{
	MovingEntity* c = static_cast<MovingEntity*>(lua_touserdata(state, lua_upvalueindex(1)));

	lua_pushnumber(state, c->velocity.x);
	lua_pushnumber(state, c->velocity.y);

	return 2;
}

int MovingEntity::LuaSetPosition(lua_State* state)
{
	MovingEntity* c = static_cast<MovingEntity*>(lua_touserdata(state, lua_upvalueindex(1)));

	c->setPosition(lua_tonumber(state, -2) + c->getPos().x, lua_tonumber(state, -1) + c->getPos().y);

	lua_pop(state, 2);

	return 0;
}
