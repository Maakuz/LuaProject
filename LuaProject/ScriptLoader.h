#ifndef SCRIPTLOADER_H
#define SCRIPTLOADER_H

extern "C" {
#include "lualib.h"
#include "lua.h"
#include "lauxlib.h"
}

class ScriptLoader 
{
private:

public:
	ScriptLoader();
	virtual ~ScriptLoader();


	bool loadScript(char* path, lua_State*& state);

};

#endif