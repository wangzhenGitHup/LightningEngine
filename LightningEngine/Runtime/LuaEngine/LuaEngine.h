#pragma once

#include "RuntimePrefix.h"
#include "Plugins/Lua/lua.hpp"

#define Lua_GetFunction(func) LightningEngine::LuaEngine::PrepareMethod(func)
#define Lua_PushNil(c) LightningEngine::LuaEngine::PushNil(c)
#define Lua_PushUserData(userdata) LightningEngine::LuaEngine::PushUserData(userdata)
#define Lua_NewTable() LightningEngine::LuaEngine::NewTable()
#define Lua_PushInt(i) LightningEngine::LuaEngine::PushInt(i)
#define Lua_PushBoolean(i) LightningEngine::LuaEngine::PushBoolean(i)
#define Lua_PushNumber(n) LightningEngine::LuaEngine::PushNumber(n)
#define Lua_PushString(str, len) LightningEngine::LuaEngine::PushString(str, len)
#define Lua_Invoke(hint) LightningEngine::LuaEngine::Invoke(hint)

namespace LightningEngine
{
	class LuaEngine
	{
	public:
		static void Init();
		static lua_State* GetLuaState();
		static void RecycleLuaState(lua_State* L);
		static int CallLibMethod(lua_State* L);
		static void CleanPlugins();
		static lua_State* Store();
		static int Restore();
		static int s_currentArgCount;
		static bool PrepareMethod(int func);
		static void PushInt(int a);
		static void PushBoolean(bool a);
		static void PushNumber(double a);
		static void PushString(const char* pstr);
		static void PushLString(const char* pstr, int len);
		static void PushNil(int count = 1);
		static void PushUserData(int userdata);
		static void NewTable();
		static void Invoke(const char* hint);
		static void DumpTable(lua_State* L);
		static int PreloadScript(lua_State* L);
		static bool GetFromCache(lua_State* L, const char* pScriptName);
		static bool ExecuteScript(lua_State* L, const char* pScriptName, const char* pCode, int codeLen);
		static bool GetLoadedScript(lua_State* L, const char* pScriptName, const char* pSpace = nullptr);
		static bool LoadScriptFromPreloadedBuffers(lua_State* L, const char* pScriptName);
		static bool LoadScriptFromUserPath(lua_State* L, const char* pScriptName);
		static void UnloadScriptCode(lua_State* L, const char* pScriptName);
		static bool LoadScriptCode(lua_State* L, const char* pScriptName, const char* pCode, int codeLen);
		static const char* GetLuaPathViaGUID(const char* pGuid);
		static bool LoadInternalScript(lua_State* L, const char* pScriptName);
		static bool GetLoadedTable(lua_State* L, const char* pTableName, bool bExist = false);
		static lua_State* s_GlobalStatePtr;

	private:
		static int s_statckSize;
		static int s_traceBackFunc;
		static std::stack<lua_State*> s_statePool;
		static std::unordered_map<std::string, std::string> s_scriptIdToPath;
		static std::stack<int> s_luaStackState;
		static std::unordered_map<std::string, HMODULE> s_pluginsMap;
	};
}

void SetEntryScript(const char* entry_script_bundle, const char* init_script);
void* GetLuaVM();