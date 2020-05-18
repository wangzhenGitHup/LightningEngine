#include "LuaCallback.h"
#include "String/FixedString.h"

namespace LightningEngine
{
	void Param::PushValue(lua_State* L)
	{
		switch (type_)
		{
		case kInt:
			lua_pushinteger(L, intValue_);
			break;

		case kFloat:
			lua_pushnumber(L, floatValue_);
			break;

		case kString:
			lua_pushstring(L, ((FixedString*)void_ptr_value_)->text_);
		}
	}
}