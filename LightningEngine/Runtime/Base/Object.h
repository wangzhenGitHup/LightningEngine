#pragma once

#include "RuntimePrefix.h"
#include "LuaEngine/LuaEngine.h"
#include "String/FixedString.h"

#define DEFINE_TYPE_ID(T, ID) const int TYPEID_##T=ID;
#define ENGINE_TYPE_ID(T) TYPEID_##T

#include "ClassID.h"


#define DECLEAR_ENGINE_CLASS(T) \
virtual int GetClassID(){return ENGINE_TYPE_ID(T);} \
static int API_ClassID(lua_State* L){lua_pushinteger(L, ENGINE_TYPE_ID(T)); return 1;} \
static const int ClassID = ENGINE_TYPE_ID(T); \
static const char* GetTypeName(){return #T;} \
static bool Is##T(lua_State*L, int idx) \
{	\
	LightningEngine::LuaEngine::Store(); \
	if(Object::IsValid(L, idx)) \
	{ \
		lua_getfield(L, idx, "ClassID"); \
		if(lua_isfunction(L, -1)) \
		{ \
			lua_pcall(L, 0, 1, 0); \
			int classID = lua_tointeger(L, -1); \
			if(classID == ClassID) \
			{ \
				LightningEngine::LuaEngine::Restore(); \
				return true; \
			} \
			else \
			{ \
				LightningEngine::LuaEngine::Restore(); \
				return false; \
			} \
		} \
		else \
		{ \
			LightningEngine::LuaEngine::Restore(); \
			return false; \
		} \
	} \
	else \
	{ \
		LightningEngine::LuaEngine::Restore(); \
		return false; \
	} \
	LightningEngine::LuaEngine::Restore(); \
	return false; \
}

#define CreateLuaDataViaExistObject(ptr, T, L) \
do \
{ \
	T**pp = (T**)lua_newuserdata(L, sizeof(T*)); \
	*pp = *ptr; \
	SET_LUA_TYPE(L, T); \
	ptr->luaData_ = luaL_ref(L, LUA_REGISTRYINDEX); \
}while(0)


namespace LightningEngine
{
	class Object
	{
	public:
		Object(int autorelease = 1);
		virtual ~Object();
		void retain();
		void release();
		void GetUserData(lua_State* L);
		void PushUserData(lua_State* L)
		{
			if (luaData_ != LUA_REFNIL)
			{
				lua_getref(L, luaData_);
			}
			else
			{
				lua_pushnil(L);
			}
		}

		template<class T>
		T* CreateExportedEngineType()
		{
			lua_State* L = LuaEngine::Store();
			T::New_ExportedLightningEngineType(L);
			T* pObj = (*(T**)lua_touserdata(L, -1));
			LuaEngine::Restore();
			return pObj;
		}

		static int DefaultGC(lua_State* L);
		static bool IsValid(lua_State* L, int idx);
		DECLEAR_ENGINE_CLASS(Object)

	public:
		Int32 referenceCount_;
		int luaData_;
		int objectMask_;
		FixedString name_;
	};
}
