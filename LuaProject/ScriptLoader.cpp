#include "ScriptLoader.h"

ScriptLoader::ScriptLoader()
{
}

ScriptLoader::~ScriptLoader()
{
}

bool ScriptLoader::loadScript(char* path, lua_State*& state)
{
	bool res = true;

	if (luaL_loadfile(state, path) || lua_pcall(state, 0, 0, 0))
		res = false;

	return res;
}
