#include "LuaEngine.h"
#include "Debugger/Log.h"
#include "Runtime/String/StringUtils.h"


#if LIGHTNING_ENGINE_WIN_PLAYER
#pragma comment(lib, "Lua5.1.4.lib")
#pragma comment(lib, "LibProtoBufferLite.lib")
#pragma comment(lib, "LibPBC.lib")
#pragma comment(lib, "LuaSocket.lib")
#endif

static char* sEntry = nullptr;
static char* sEntryScriptBuddle = nullptr;

lua_State* LightningEngine::LuaEngine::s_GlobalStatePtr = nullptr;
int LightningEngine::LuaEngine::s_currentArgCount = 0;

void SetEntryScript(const char * entry_script_bundle, const char * init_script)
{
	if (entry_script_bundle != nullptr)
	{
		sEntryScriptBuddle = new char[256];
		memset(sEntryScriptBuddle, 0, 256);
		strcpy(sEntryScriptBuddle, entry_script_bundle);
	}

	if (init_script != nullptr)
	{
		sEntry = new char[256];
		memset(sEntry, 0, 256);
		strcpy(sEntry, init_script);
	}
}

void * GetLuaVM()
{
	return LightningEngine::LuaEngine::s_GlobalStatePtr;
}

namespace LightningEngine
{
	int LuaEngine::s_traceBackFunc = LUA_REFNIL;
	std::stack<lua_State*> LuaEngine::s_statePool;
	std::unordered_map<std::string, std::string> LuaEngine::s_scriptIdToPath;
	int LuaEngine::s_statckSize = 0;
	std::stack<int> LuaEngine::s_luaStackState;
	std::unordered_map<std::string, HMODULE> LuaEngine::s_pluginsMap;

	void LuaEngine::Init()
	{
		s_GlobalStatePtr = luaL_newstate();
		luaL_openlibs(s_GlobalStatePtr);
		lua_getglobal(s_GlobalStatePtr, "debug");
		lua_getfield(s_GlobalStatePtr, -1, "traceback");
		s_traceBackFunc = luaL_ref(s_GlobalStatePtr, LUA_REGISTRYINDEX);
		lua_settop(s_GlobalStatePtr, 0);
	}

	lua_State * LuaEngine::GetLuaState()
	{
		lua_State* ret = s_statePool.top();
		s_statePool.pop();
		return ret;
	}

	void LuaEngine::RecycleLuaState(lua_State * L)
	{
		s_statePool.push(L);
	}

	int LuaEngine::CallLibMethod(lua_State * L)
	{
		if (!lua_isstring(L, 1))
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "first arg must be a string value to specify lib path relative to Assets Folder");
			return 2;
		}

		if (!lua_isstring(L, 2))
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "second arg must be a string value to specify method name");
			return 2;
		}

#if LIGHTNING_ENGINE_WIN_PLAYER
		const char* libName = lua_tostring(L, 1);
		const char* method = lua_tostring(L, 2);
		std::unordered_map<std::string, HMODULE>::iterator iter = s_pluginsMap.find(libName);
		HMODULE module = nullptr;
		if (iter == s_pluginsMap.end())
		{
			module = LoadLibraryA(libName);
			if (nullptr != module)
			{
				s_pluginsMap.insert(std::pair<std::string, HMODULE>(libName, module));
			}
			else
			{
				lua_pushboolean(L, false);
				lua_pushstring(L, "load lib fail");
				return 1;
			}
		}
		else
		{
			module = iter->second;
		}

		lua_CFunction func = (lua_CFunction)GetProcAddress(module, method);
		if (nullptr != func)
		{
			lua_pushcfunction(L, func);
			if (lua_pcall(L, 0, 0, 0) != 0)
			{
				lua_pushboolean(L, false);
				lua_pushvalue(L, -2);
			}
			else
			{
				lua_pushboolean(L, true);
				lua_pushstring(L, "ok");
			}
		}
		else
		{
			lua_pushboolean(L, false);
			char szBuffer[256] = { 0 };
			sprintf(szBuffer, "cannot find %s in lib %s", method, libName);
			lua_pushstring(L, szBuffer);
		}
#endif
		return 2;
	}

	void LuaEngine::CleanPlugins()
	{
#if LIGHTNING_ENGINE_WIN_PLAYER
		std::unordered_map<std::string, HMODULE>::iterator iter = s_pluginsMap.begin();
		for (; iter != s_pluginsMap.end(); )
		{
			FreeLibrary(iter->second);
			iter = s_pluginsMap.erase(iter);
		}
#endif

	}

	lua_State * LuaEngine::Store()
	{
		s_luaStackState.push(lua_gettop(s_GlobalStatePtr));
		lua_getref(s_GlobalStatePtr, s_traceBackFunc);
		return s_GlobalStatePtr;
	}

	int LuaEngine::Restore()
	{
		int topSize = s_luaStackState.top();
		lua_settop(s_GlobalStatePtr, topSize);
		s_luaStackState.pop();
		return topSize;
	}
	bool LuaEngine::PrepareMethod(int func)
	{
		lua_State* L = Store();
		lua_getref(L, func);
		if (lua_isfunction(L, -1))
		{
			s_currentArgCount = 0;
			return true;
		}

		Restore();
		return false;
	}

	void LuaEngine::PushInt(int a)
	{
		s_currentArgCount++;
		lua_pushinteger(s_GlobalStatePtr, a);
	}

	void LuaEngine::PushBoolean(bool a)
	{
		s_currentArgCount++;
		lua_pushboolean(s_GlobalStatePtr, a);
	}

	void LuaEngine::PushNumber(double a)
	{
		s_currentArgCount++;
		lua_pushnumber(s_GlobalStatePtr, a);
	}

	void LuaEngine::PushString(const char * pstr)
	{
		s_currentArgCount++;
		lua_pushstring(s_GlobalStatePtr, pstr);
	}

	void LuaEngine::PushLString(const char * pstr, int len)
	{
		s_currentArgCount++;
		lua_pushlstring(s_GlobalStatePtr, pstr, len);
	}

	void LuaEngine::PushNil(int count)
	{
		while (count > 0)
		{
			lua_pushnil(s_GlobalStatePtr);
			s_currentArgCount++;
			count--;
		}
	}

	void LuaEngine::PushUserData(int userdata)
	{
		s_currentArgCount++;
		if (LUA_REFNIL == userdata)
		{
			lua_pushnil(s_GlobalStatePtr);
		}
		else
		{
			lua_getref(s_GlobalStatePtr, userdata);
		}
	}

	void LuaEngine::NewTable()
	{
		s_currentArgCount++;
		lua_newtable(s_GlobalStatePtr);
	}

	void LuaEngine::Invoke(const char * hint)
	{
		int error_func_idx = -2 - s_currentArgCount;
		if (lua_pcall(s_GlobalStatePtr, s_currentArgCount, 0, error_func_idx) != 0)
		{
			ErrorPrint(hint, lua_tostring(s_GlobalStatePtr, -1));
		}

		s_currentArgCount = 0;
		Restore();
	}

	void LuaEngine::DumpTable(lua_State * L)
	{
		if (!lua_istable(L, -1))
		{
			ErrorPrint("top is not a table");
			return;
		}

		lua_pushnil(L);
		while (lua_next(L, -2))
		{
			lua_getglobal(L, "tostring");
			lua_pushvalue(L, -3);
			const char* pKey = "unknown";
			if (lua_pcall(L, 1, 1, 0) == 0)
			{
				pKey = lua_tostring(L, -1);
			}

			lua_getglobal(L, "tostring");
			lua_pushvalue(L, -3);
			const char* pValue = "unknown";
			if (lua_pcall(L, 1, 1, 0) == 0)
			{
				pValue = lua_tostring(L, -1);
			}

			Debug("%s : %s", pKey, pValue);
			lua_pop(L, 3);
		}
	}

	int LuaEngine::PreloadScript(lua_State * L)
	{
		if (lua_gettop(L) != 2)
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "arg count must be 2");
			return 2;
		}

		if (!lua_isstring(L, 1) || !lua_isstring(L, 2))
		{
			lua_pushboolean(L, false);
			lua_pushstring(L, "arg must be 2 string");
			return 2;
		}

		const char* pScriptName = lua_tostring(L, 1);
		const char* pCode = lua_tostring(L, 2);
		LoadScriptCode(L, pScriptName, pCode, strlen(pCode));
		
		return 1;
	}

	bool LuaEngine::GetFromCache(lua_State * L, const char * pScriptName)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
		lua_getfield(L, -1, "Assets");
		if (lua_istable(L, -1))
		{
			lua_getfield(L, -1, pScriptName);
			if (lua_isnil(L, -1))
			{
				lua_pop(L, 3);
			}
			else
			{
				lua_remove(L, -2);
				lua_remove(L, -2);
				return true;
			}
		}
		else
		{
			lua_pop(L, 2);
		}
		return false;
	}

	bool LuaEngine::ExecuteScript(lua_State * L, const char * pScriptName, const char * pCode, int codeLen)
	{
		if (luaL_loadbuffer(L, pCode, codeLen, pScriptName) == 0)
		{
			if (lua_pcall(L, 0, 1, -4) == 0)
			{
				if (lua_istable(L, -1))
				{
					lua_pushstring(L, pScriptName);
					lua_setfield(L, -2, "Name");
					lua_setfield(L, -2, pScriptName);
					lua_getfield(L, -1, pScriptName);
					lua_remove(L, -2);
					lua_remove(L, -2);
					lua_remove(L, -2);
				}
				else
				{
					lua_pop(L, 4);
					lua_pushnil(L);
				}

				return true;
			}
			else
			{
				lua_pop(L, 4);
				lua_pushnil(L);
			}
		}
		else
		{
			lua_pop(L, 4);
			lua_pushnil(L);
		}

		return false;
	}

	bool LuaEngine::GetLoadedScript(lua_State * L, const char * pScriptName, const char * pSpace)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
		if (nullptr == pSpace)
		{
			lua_getfield(L, -1, pScriptName);
			if (!lua_isnil(L, -1))
			{
				lua_remove(L, -2);
				return true;
			}
			
			lua_pop(L, 2);
		}
		else
		{
			lua_getfield(L, -1, pSpace);
			if (lua_istable(L, -1))
			{
				lua_getfield(L, -1, pScriptName);
				if (lua_isnil(L, -1))
				{
					lua_pop(L, 3);
				}
				else
				{
					lua_remove(L, -2);
					lua_remove(L, -2);
					return true;
				}
			}
			else
			{
				lua_pop(L, 2);
			}
		}
		return false;
	}

	bool LuaEngine::LoadScriptFromPreloadedBuffers(lua_State * L, const char * pScriptName)
	{
#if LIGHTNING_ENGINE_EDITOR || (LIGHTNING_ENGINE_WIN_PLAYER && _DEBUG)
#else
#endif
		return false;
	}
	bool LuaEngine::LoadScriptFromUserPath(lua_State * L, const char * pScriptName)
	{
		char luaPath[256] = { 0 };
		strcpy(luaPath, pScriptName);
		StringUtils::LuaPathToSTDPath(luaPath);
		strcat(luaPath, ".lua");
		/*Data luaSrc;
		if (FileSystem::LoadDataFromPath(luaPath, luaSrc))
		{
			LoadScriptCode(L, pScriptName, (char*)luaSrc.data_, luaSrc.dataLen_);
			return true;
		}*/

		return false;
	}

	void LuaEngine::UnloadScriptCode(lua_State * L, const char * pScriptName)
	{
		Store();
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
		lua_getfield(L, -1, "Assets");
		lua_pushnil(L);
		lua_setfield(L, -2, pScriptName);
		Restore();
	}

	bool LuaEngine::LoadScriptCode(lua_State * L, const char * pScriptName, const char * pCode, int codeLen)
	{
		lua_getref(L, s_traceBackFunc);
		GetLoadedTable(L, "Assets", true);
		return ExecuteScript(L, pScriptName, pCode, codeLen);
	}

	const char * LuaEngine::GetLuaPathViaGUID(const char * pGuid)
	{
		std::unordered_map<std::string, std::string>::iterator iter = s_scriptIdToPath.find(pGuid);
		if (iter != s_scriptIdToPath.end())
		{
			return iter->second.c_str();
		}

		return nullptr;
	}

	bool LuaEngine::LoadInternalScript(lua_State * L, const char * pScriptName)
	{
		if (GetFromCache(L, pScriptName))
		{
			return true;
		}

		char luaPath[256] = { 0 };
		strcpy(luaPath, pScriptName);
		StringUtils::LuaPathToSTDPath(luaPath);
		strcat(luaPath, ".lua");
		/*Data luaSrc;
		if (ResourceManager::LoadData(luaPath, luaSrc))
		{
			LoadScriptCode(L, pScriptName, (char*)luaSrc.data_, luaSrc.dataLen_);
			return true;
		}*/

		ErrorPrint("cannot locate script %s : %s", pScriptName, luaPath);
		return false;
	}

	bool LuaEngine::GetLoadedTable(lua_State * L, const char * pTableName, bool bExist)
	{
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
		lua_getfield(L, -1, "Assets");
		if (lua_isnil(L, -1) && bExist)
		{
			lua_pop(L, 1);
			lua_newtable(L);
			lua_setfield(L, -2, "Assets");
			return true;
		}

		return false;
	}
}


