#include "Player.h"

Player::Player(sf::Texture* tex, int width, int height, sf::Vector2i sheetSize, int animTime)
	:AnimatedEntity(tex, width, height, sheetSize, animTime)
{
	this->alive = true;
}

Player::~Player()
{
}

void Player::setAudioHandler(AudioHandler* audioHandler)
{
	this->audioHandler = audioHandler;
}

void Player::setupLuaFunctions(lua_State* state)
{
	this->addCFunction(state, LuaHandleInputs, "handleInputs", this);
	MovingEntity::setupLuaFunctions(state);
}

void Player::collisionEnemy(ArrayList<Enemy>& enemies)
{
	bool immunity = false;
	for (int i = 0; i < enemies.size() && !immunity; i++)
	{
		if (this->getVelocity().y >= 0 && this->getBox()->getBottom().intersects(enemies[i].getBox()->getTop()))
		{
			enemies.removeAt(i);
			this->setVelocity(this->getVelocity().x, this->getJumpHeight());
			immunity = true;
			audioHandler->callLuaFunc("playHitSound");
		}

		else if (this->getBox()->getRect().intersects(enemies[i].getBox()->getRect()))
		{
			this->alive = false; 
		}
	}
}

void Player::collisionWorldSides()
{
	if (this->getPos().y > 768)
		this->alive = false;

	if (this->getPos().x < 0)
		this->setPosition(0, this->getPos().y);
}

bool Player::collisionObject(ArrayList<Entity>& objects)
{
	bool res = false;
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].getBox()->getRect().intersects(this->getBox()->getRect()))
		{
			objects.removeAt(i);
			res = true;
		}
	}

	return res;
}

void Player::move()
{
	this->setDirection(0);
	this->setMoving(false);

	if (ks.isKeyPressed(ks.D))
	{
		this->setDirection(1);
		this->setMoving(true);
	}

	if (ks.isKeyPressed(ks.A))
	{
		this->setDirection(-1);
		this->setMoving(true);
	}

	if (ks.isKeyPressed(ks.W) && !this->getIsJumping())
	{
		this->setPosition(this->getPos().x, this->getPos().y - 10);
		this->jump(this->getJumpHeight());
		this->audioHandler->callLuaFunc("playJumpSfx");
	}
}

bool Player::isAlive() const
{
	return this->alive;
}

//***************************************************************
//*************************LUA FUNCTIONS*************************
//***************************************************************
int Player::LuaHandleInputs(lua_State* state)
{
	Player* c = static_cast<Player*>(lua_touserdata(state, lua_upvalueindex(1)));
	c->move();

	return 0;
}
