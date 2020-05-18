#pragma once

#include "Plugins/Lua/lua.hpp"
#include "Runtime/Utils/LinkedList.h"

namespace LightningEngine
{
	class LuaCallback
	{
	public:
		int luaTable_;
		int luaFunc_;
	};

	class Param : public LinkedList
	{
	public:
		enum
		{
			kInt,
			kFloat,
			kString,
			kUnknow
		};

		int type_;
		void* void_ptr_value_;
		int intValue_;
		float floatValue_;

		Param() : type_(kUnknow) {}

		void PushValue(lua_State* L);
	};
}