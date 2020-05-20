#include "Object.h"

namespace LightningEngine
{
	Object::Object(int autorelease) : 
		referenceCount_(0), 
		objectMask_(autorelease)
	{
	}

	Object::~Object()
	{
	}

	void Object::retain()
	{
		referenceCount_++;
	}

	void Object::release()
	{
		referenceCount_--;
		if (0 == referenceCount_)
		{
			if (objectMask_ == 1)
			{
				delete this;
			}
		}
	}

	void Object::GetUserData(lua_State * L)
	{
		luaData_ = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_getref(L, luaData_);
	}

	int Object::DefaultGC(lua_State * L)
	{
		Object* pObj = TO_USERDATA(L, Object, -1);
		pObj->release();
		return 0;
	}

	bool Object::IsValid(lua_State * L, int idx)
	{
		bool bRet = false;
		lua_getmetatable(L, idx);
		if (lua_istable(L, -1))
		{
			lua_pop(L, 1);
			bRet = true;
		}
		return bRet;
	}
}