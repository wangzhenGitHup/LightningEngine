#pragma once

extern "C" {
#include "External/Lua/LuaVMLib/lua.h"
#include "External/Lua/LuaVMLib/lualib.h"
#include "External/Lua/LuaVMLib/lauxlib.h"
}

#define MODULE_NAME(N) #N
#define METHOD_NAME(N) #N
#define EXPORT_NAME(N) "LightningEngine."#N
#define METATABLENAME(T) "MT_"#T

#if LIGHTNING_ENGINE_PLATFORM_WIN || LIGHTNING_ENGINE_EDITOR
#define LIGHTNING_ENGINE_MODULE(X) LightningEngine::##X
#else
#define LIGHTNING_ENGINE_MODULE(X) LightningEngine::X
#endif

#define TO_USERDATA(L, t, idx) (*(t**)lua_touserdata(L, idx))

#define LUA_TYPE(l, i) lua_typename(l, lua_type(l, i))
#define API_NAME(name) API_##name
#define DEFINE_LUA_API(name) static int API_##name(lua_State* L)
#define EXPORT_API(name) {#name, API_##name},
#define EXPORT_API_NEW {"New", New_ExportedLightningEngineType},
#define EXPORT_API_END {NULL, NULL}


#define SET_PARENT(L, parent) do \
{\
	luaL_getmetatable(L, METATABLENAME(parent)); \
	lua_setmetatable(L, -2); \
}while(0)


#define SET_LUA_TYPE(L, t) SET_PARENT(L, t)

#define UNSET_LUA_TYPE(L) do { \
	lua_pushnil(L); \
	lua_setmetatable(L, -2); \
}while(0)


#define UNREF(L, obj) \
do{if(LUA_REFNIL != obj){lua_unref(L, obj); obj = LUA_REFNIL;}}while(0)


#define REF_FIELD(L, idx, obj, name) \
do{ \
	lua_getfield(L, idx, name); \
	if(!lua_isnil(L, -1)) { \
		obj = luaL_ref(L, LUA_REGISTRYINDEX); \
	}else{\
		lua_pop(L, 1);\
	}\
}while(0)


#define REF_TOP(L, obj) \
do{if(LUA_REFNIL != obj){lua_unref(L, obj}; obj = LUA_REFNIL;}}while(0)


#define SAFE_INVOKE_LUA_METHOD_ARG0(m)do{\
	if(m != LUA_REFNIL){lua_State* L = LightningEngine::Store(); lua_getref(L, m); \
	if(lua_isfunction(L, -1)){if(lua_pcall(L, 0, 0, -2) != 0){ErrorPrint("%s", lua_tostring(L, -1));} \
}LightningEngine::LuaEngine::Restore();}}while(0)

#define SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(t, m)do{ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, t); \
	if(lua_istable(L, -1)){ \
		lua_getfield(L, -1, METHOD_NAME(m)); \
		if(lua_isfunction(L, -1)){ \
			lua_getref(L, t); \
			if(lua_pcall(L, 1, 0, -4) != 0){ \
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define SAFE_INVOKE_LUA_PACKAGE_METHOD_ARG0(t, m) do{\
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, t); \
	if(lua_istable(L, -1)){ \
		if(lua_isfunction(L, -1)){\
			if(lua_pcall(L, 0, 0, -2) != 0){\
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define FAST_SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(t, m) do{ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, t); \
	if(lua_istable(L, -1)){\
		lua_getref(L, m); \
		if(lua_isfunction(L, -1)){ \
			lua_getref(L, t); \
			if(lua_pcall(L, 1, 0, -3) != 0){\
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define SAFE_INVOKE_LUA_METHOD_ARG1F(m, arg1) do{ \
	lua_State* L = LightningEngine::LuaEngine:Store(); \
	lua_getref(L, m); \
	if (lua_isfunction(L, -1)) { \
		lua_pushnumber(L, arg1); \
		if (lua_pcall(L, 1, 0, -3) != 0) { \
			ErrorPrint("%s", lua_tostring(L, -1)); \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define SAFE_INVOKE_LUA_CLASS_METHOD_ARG1F(t, m, arg1)do{ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, t); \
	if (lua_istable(L, -1)) { \
		lua_getfield(L, -1, MEHTOD_NAME(m)); \
		if (lua_isfunction(L, -1)) { \
			lua_getref(L, t); \
			lua_pushnumber(L, arg1); \
			if (lua_pcall(L, 2, 0, -4) != 0) { \
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define SAFE_INVOKE_LUA_PACKAGE_METHOD_ARG1F(t, m, arg1)do{ \
	lua_State* L = LightningEngine::luaEngine::Store(); \
	lua_getref(L, t); \
	if (lua_istable(L, -1)) { \
		lua_getfield(L, -1, METHOD_NAME(m)); \
		if (lua_isfunction(L, -1)) { \
			lua_pushnumber(L, arg1); \
			if (lua_pcall(L, 1, 0, -3) != 0) { \
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define FAST_SAFE_INVOKE_LUA_CLASS_METHOD_ARG1F(t, m, arg1)do{ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, m); \
	if (lua_isfunction(L, -1)) { \
		lua_getref(L, t); \
		lua_pushnumber(L, arg1); \
		if (lua_pcall(L, 2, 0, -4) != 0) { \
			ErrorPrint("%s", lua_tostring(L, -1)); \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define FAST_SAFE_INVOKE_LUA_PACKAGE_METHOD_ARG1F(t, m, arg1)do{ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, t); \
	if (lua_istable(L, -1)) { \
		lua_getref(L, m); \
		if (lua_isfunction(L, -1)) { \
			lua_pushnumber(L, arg1); \
			if (lua_pcall(L, 1, 0, -3) != 0) { \
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define SAFE_INVOKE_LUA_CLASS_METHOD_ARG2F(t, m, arg1, arg2)do{ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, t); \
	if (lua_istable(L, -1)) { \
		lua_getfield(L, -1, METHOD_NAME(m)); \
		if (lua_isfunction(L, -1)) { \
			lua_getref(L, t); \
			lua_pushnumber(L, arg1); \
			lua_pushnumber(L, arg2); \
			if (lua_pcall(L, 3, 0, -6) != 0) { \
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define FAST_SAFE_INVOKE_LUA_CLASS_METHOD_2I(t, m, arg1, arg2)do{ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, t); \
	if (lua_istable(L, -1)) { \
		lua_getref(L, m); \
		if (lua_isfunction(L, -1)) { \
			lua_getref(L, t); \
			lua_pushinteger(L, arg1); \
			lua_pushinteger(L, arg2); \
			if (lua_pcall(L, 3, 0, -6) != 0) { \
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(t, m, a1, a2, a3)do{ \
	if(m != LUA_REFNIL){ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, m); \
	lua_getref(L, t); \
	lua_pushnumber(L, a1); \
	lua_pushnumber(L, a2); \
	lua_pushinteger(L, Int32(a3)); \
	if (lua_pcall(L, 4, 0, -6) != 0) { \
		ErrorPrint("%s", lua_tostring(L, -1)); \
	} \
	LightningEngine::LuaEngine::Restore(); \
  } \
}while(0)


#define SAFE_INVOKE_LUA_CLASS_METHOD_1I(t, m, a1)do{ \
	if(m != LUA_REFNIL){ \
		lua_State* L = LightningEngine::LuaEngine::Store(); \
		lua_getref(L, m); \
		lua_getref(L, t); \
		lua_pushinteger(L, a1); \
		if (lua_pcall(L, 2, 0, -4) != 0) { \
			ErrorPrint("%s", lua_tostring(L, -1)); \
		} \
		LightningEngine::LuaEngine::Restore(); \
	} \
}while(0)


#define SAFE_INVOKE_LUA_CLASS_METHOD_2I(t, m, arg1, arg2)do{ \
	lua_State* L = LightningEngine::LuaEngine::Store(); \
	lua_getref(L, t); \
	if (lua_istable(L, -1)) { \
		lua_getfield(L, -1, METHOD_NAME(m)); \
		if (lua_isfunction(L, -1)) { \
			lua_getref(L, t); \
			lua_pushinteger(L, arg1); \
			lua_pushiteger(L, arg2); \
			if (lua_pcall(L, 3, 0, -6) != 0) { \
				ErrorPrint("%s", lua_tostring(L, -1)); \
			} \
		} \
	} \
	LightningEngine::LuaEngine::Restore(); \
}while(0)


#define CheckArgCount(n, l, c) \
if(lua_gettop(l) < c){ \
	char szBuffer[256] = { 0 }; \
	sprintf(szBuffer, "%s need more args, now arg count is %d", #n, lua_gettop(l)); \
	lua_pushboolean(l, false); \
	lua_pushstring(l, szBuffer); \
	return 2; \
}


#define  NEW_LUA_TYPE_WITH_API(L, t, mf)do{ \
	luaL_newmetatable(L, METATABLENAME(t)); \
	lua_pushstring(L, MODULE_NAME(t)); \
	lua_setfield(L, -2, "Name"); \
	lua_pushvalue(L, -1); \
	lua_setfield(L, -2, "__index"); \
	luaL_openlib(L, NULL, mf, 0); \
	lua_pushcfunction(L, t::DefaultGC); \
	lua_setfield(L, -2, "__gc"); \
}while(0) 


#define DEF_LIGHTNINGENGINE_EXPORTED_TYPE_NEW_METHOD(cpp_type, luatype) static int New_ExportedLightningEngineType(lua_State* L) \
{ \
	cpp_type** pp = (cpp_type**)lua_newuserdata(L, sizeof(cpp_type*)); \
	*pp = new (kMemDefaultId) cpp_type; \
	cpp_type* pObject = *pp; \
    SET_LUA_TYPE(L, luatype); \
    pObject->GetUserData(L); \
    return 1; \
}