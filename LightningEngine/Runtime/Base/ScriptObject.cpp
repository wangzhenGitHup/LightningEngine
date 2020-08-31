#include "ScriptObject.h"
#include "LuaEngine/LuaEngine.h"
#include "Scene/GameObject.h"
#include "Scene/EventDispatcher.h"
#include "String/StringUtils.h"
#include "IO/ResourceManager.h"
#include "Scene/TouchEvent.h"
#include "Debugger/Log.h"


namespace LightningEngine
{
	ScriptObject::ScriptObject() : 
		update_(LUA_REFNIL),
		fixedUpdate_(LUA_REFNIL),
		onChar_(LUA_REFNIL),
		onIMEChar_(LUA_REFNIL),
		onTouchBegin_(LUA_REFNIL),
		onTouchEnd_(LUA_REFNIL),
		onTouchMove_(LUA_REFNIL),
		onTouchCanceled_(LUA_REFNIL),
		onMouseWheel_(LUA_REFNIL),
		onTouchEnter_(LUA_REFNIL),
		onTouchLeave_(LUA_REFNIL),
		onKeyUp_(LUA_REFNIL),
		onKeyDown_(LUA_REFNIL),
		animationEnd_(LUA_REFNIL),
		onPostRendering_(LUA_REFNIL)
	{
	}

	ScriptObject::~ScriptObject()
	{
		lua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "ScriptObject");
		lua_pushnil(L);
		lua_setmetatable(L, -2);
		lua_pop(L, 1);
		lua_pushnil(L);
		lua_setfield(L, -2, "ScriptObject");
		lua_pushnil(L);
		lua_setfield(L, -2, "GameObject");
		lua_unref(L, luaData_);

		luaData_ = LUA_REFNIL;
		LuaEngine::Restore();
		OnDestroyScript();
	}

	int ScriptObject::Export(lua_State * L)
	{
		luaL_Reg apim[] = {
			{NULL, NULL}
		};

		NEW_LUA_TYPE_WITH_API(L, ScriptObject, apim);
		SET_PARENT(L, Component);
		return 0;
	}

	int ScriptObject::DefaultGC(lua_State * L)
	{
		Debug("ScriptObject::DefaultGC");
		return 0;
	}

	void ScriptObject::Awake(lua_State * L)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(luaData_, Awake);
	}

	void ScriptObject::OnInitScript()
	{
		lua_State* L = LuaEngine::Store();
		REF_FIELD(L, -2, update_, "Update");
		REF_FIELD(L, -2, fixedUpdate_, "FixedUpdate");
		REF_FIELD(L, -2, onChar_, "OnChar");
		REF_FIELD(L, -2, onIMEChar_, "OnIMEChar");
		REF_FIELD(L, -2, onTouchBegin_, "OnTouchBegin");
		REF_FIELD(L, -2, onTouchEnd_, "OnTouchEnd");
		REF_FIELD(L, -2, onTouchMove_, "OnTouchMove");
		REF_FIELD(L, -2, onTouchCanceled_, "OnTouchCanceled");
		REF_FIELD(L, -2, onTouchEnter_, "OnTouchEnter");
		REF_FIELD(L, -2, onTouchLeave_, "OnTouchLeave");
		REF_FIELD(L, -2, onMouseWheel_, "OnMouseWheel");
		REF_FIELD(L, -2, onKeyUp_, "OnKeyUp");
		REF_FIELD(L, -2, onKeyDown_, "OnKeyDown");
		REF_FIELD(L, -2, onPostRendering_, "OnPostRendering");

		LuaEngine::Restore();
	}

	void ScriptObject::OnDestroyScript()
	{
		lua_State *L = LuaEngine::Store();
		UNREF(L, update_);
		UNREF(L, fixedUpdate_);
		UNREF(L, onChar_);
		UNREF(L, onIMEChar_);
		UNREF(L, onTouchBegin_);
		UNREF(L, onTouchEnd_);
		UNREF(L, onTouchMove_);
		UNREF(L, onTouchEnter_);
		UNREF(L, onTouchLeave_);
		UNREF(L, onMouseWheel_);
		UNREF(L, onKeyUp_);
		UNREF(L, onKeyDown_);
		UNREF(L, animationEnd_);
		UNREF(L, onPostRendering_);

		LuaEngine::Restore();
	}

	bool ScriptObject::GetMethod(lua_State * L, const char * methodName)
	{
		if (luaData_ != LUA_REFNIL)
		{
			lua_getref(L, luaData_);
			lua_getfield(L, -1, methodName);
			if (!lua_isnil(L, -1))
			{
				lua_remove(L, -2);
				return true;
			}

			lua_pop(L, 2);
			return false;
		}

		return false;
	}

	void ScriptObject::Update(float deltaTime)
	{
		FAST_SAFE_INVOKE_LUA_CLASS_METHOD_ARG1F(luaData_, update_, deltaTime);
	}

	void ScriptObject::FixedUpdate(float deltaTime)
	{
		FAST_SAFE_INVOKE_LUA_CLASS_METHOD_ARG1F(luaData_, fixedUpdate_, deltaTime);
	}

	void ScriptObject::OnTouchBegin(const TouchEvent & touch)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(luaData_, onTouchBegin_, touch.x_, touch.y_, touch.touchID_);
	}

	void ScriptObject::OnTouchEnd(const TouchEvent & touch)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(luaData_, onTouchEnd_, touch.x_, touch.y_, touch.touchID_);
	}

	void ScriptObject::OnTouchCanceled(const TouchEvent & touch)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(luaData_, onTouchCanceled_, touch.x_, touch.y_, touch.touchID_);
	}

	void ScriptObject::OnTouchMove(const TouchEvent & touch)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(luaData_, onTouchMove_, touch.x_, touch.y_, touch.touchID_);
	}

	void ScriptObject::OnTouchEnter(const TouchEvent & touch)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(luaData_, onTouchEnter_, touch.x_, touch.y_, touch.touchID_);
	}

	void ScriptObject::OnTouchLeave(const TouchEvent & touch)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_2F_1I(luaData_, onTouchLeave_, touch.x_, touch.y_, touch.touchID_);
	}

	//void ScriptObject::OnTouchWheel(const MouseWheelEvent & touch)
	//{
	//	FAST_SAFE_INVOKE_LUA_CLASS_METHOD_2I(luaData_, onTouchWheel_, touch.key_, touch.wheelDelta_);
	//}

	void ScriptObject::OnMouseWheel(const MouseWheelEvent * event)
	{
		FAST_SAFE_INVOKE_LUA_CLASS_METHOD_2I(luaData_, onMouseWheel_, event->key_, event->wheelDelta_);
	}

	void ScriptObject::OnCollideBegin(GameObject * obj)
	{
		/*lua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "OnCollideBegin");
		if (!lua_isnil(L, -1))
		{
			lua_pushvalue(L, -2);
			obj->PushUserData(L);
			if (lua_pcall(L, 2, 0, 0) != 0)
			{
				ErrorPrint("ScriptObject::OnCollideBegin %s", lua_tostring(L, -1));
			}
		}
		
		LuaEngine::Restore();*/
	}

	void ScriptObject::OnCollideEnd(GameObject * obj)
	{
		/*Ulua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "OnCollideEnd");
		if (!lua_isnil(L, -1))
		{
			lua_pushvalue(L, -2);
			obj->PushUserData(L);
			if (lua_pcall(L, 2, 0, 0) != 0)
			{
				ErrorPrint("ScriptObject::OnCollideEnd %s", lua_tostring(L, -1));
			}
		}

		LuaEngine::Restore();*/
	}

	void ScriptObject::OnDestroy()
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(luaData_, OnDestroy);
	}

	void ScriptObject::OnShow()
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(luaData_, OnShow);
	}

	void ScriptObject::OnHide()
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(luaData_, OnHide);
	}

	void ScriptObject::OnViewportChange(float width, float height)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG2F(luaData_, OnViewportChange, width, height);
	}

	void ScriptObject::OnKeyUp(int code)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_1I(luaData_, onKeyUp_, code);
	}

	void ScriptObject::OnKeyDown(int code)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_1I(luaData_, onKeyUp_, code);
	}

	void ScriptObject::OnIMEChar(const char * utf8Str)
	{
		lua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "OnIMEChar");
		if (lua_isfunction(L, -1))
		{
			lua_pushvalue(L, -2);
			lua_pushstring(L, utf8Str);
			if (lua_pcall(L, 2, 0, 0) != 0)
			{
				ErrorPrint("ScriptObject::OnIMEChar %s", lua_tostring(L, -1));
			}
		}

		LuaEngine::Restore();
	}

	void ScriptObject::OnIMECompositionString(const char * utf8Str, bool bRet)
	{
		lua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "OnIMECompositionString");
		if (lua_isfunction(L, -1))
		{
			lua_pushvalue(L, -2);
			lua_pushstring(L, utf8Str);
			lua_pushboolean(L, bRet);
			if (lua_pcall(L, 3, 0, -6) != 0)
			{
				ErrorPrint("ScriptObject::OnIMECompositionString %s", lua_tostring(L, -1));
			}
		}

		LuaEngine::Restore();
	}

	void ScriptObject::OnEditEnd()
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_ARG0(luaData_, OnEditEnd);
	}

	void ScriptObject::OnChar(UInt32 charCode)
	{
		SAFE_INVOKE_LUA_CLASS_METHOD_1I(luaData_, onChar_, charCode);
	}

	void ScriptObject::OnPaste(const char * str, int len)
	{
		lua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "OnPaste");
		if (lua_isfunction(L, -1))
		{
			lua_pushvalue(L, -2);
			lua_pushlstring(L, str, len);
			if (lua_pcall(L, 2, 0, 0) != 0)
			{
				ErrorPrint("ScriptObject::OnPaste %s", lua_tostring(L, -1));
			}
		}

		LuaEngine::Restore();
	}

	void ScriptObject::OnIAP(void * product)
	{
		lua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "OnIAP");
		if (lua_isfunction(L, -1))
		{
			lua_pushvalue(L, -2);
			IAPData* iapData = (IAPData*)product;
			lua_pushlstring(L, iapData->product_.text_, iapData->product_.len_);
			lua_pushlstring(L, iapData->bill_.text_, iapData->bill_.len_);
			if (lua_pcall(L, 3, 0, 0) != 0)
			{
				ErrorPrint("ScriptObject::OnIAP %s", lua_tostring(L, -1));
			}
		}

		LuaEngine::Restore();
	}

	void ScriptObject::OnAdReward(void * product)
	{
		lua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "OnAdReward");
		if (lua_isfunction(L, -1))
		{
			lua_pushvalue(L, -2);
			AdReward* pReward = (AdReward*)product;
			lua_pushlstring(L, pReward->name_.text_, pReward->name_.len_);
			lua_pushinteger(L, pReward->count_);
			if (lua_pcall(L, 3, 0, 0) != 0)
			{
				ErrorPrint("ScriptObject::OnAdReward %s", lua_tostring(L, -1));
			}
		}

		LuaEngine::Restore();
	}

	void ScriptObject::OnAnimationEnd(const char * animationName)
	{
		lua_State* L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, "OnAnimationEnd");
		if (!lua_isnil(L, -1))
		{
			lua_pushvalue(L, -2);
			lua_pushstring(L, animationName);
			if (lua_pcall(L, 2, 0, 0) != 0)
			{
				ErrorPrint("ScriptObject::OnAnimationEnd %s", lua_tostring(L, -1));
			}
		}

		LuaEngine::Restore();
	}

	void ScriptObject::OnPostRendering(Object * colorBuffer)
	{
		if (onPostRendering_ != LUA_REFNIL)
		{
			lua_State* L = LightningEngine::LuaEngine::Store();
			lua_getref(L, onPostRendering_);
			lua_getref(L, luaData_);
			colorBuffer->PushUserData(L);
			if (lua_pcall(L, 2, 0, 0) != 0)
			{
				ErrorPrint("ScriptObject::OnPostRendering: %s", lua_tostring(L, -1));
			}

			LightningEngine::LuaEngine::Restore();
		}
	}

	void ScriptObject::Call(const char * method, Param * param)
	{
		lua_State*L = LuaEngine::Store();
		lua_getref(L, luaData_);
		lua_getfield(L, -1, method);

		if (lua_isfunction(L, -1)) 
		{
			lua_pushvalue(L, -2);
			Param*arg = param;
			int argc = 1;
			while (arg != nullptr) 
			{
				arg->PushValue(L);
				arg = arg->Next<Param>();
				argc++;
			}

			if (lua_pcall(L, argc, 0, -2 - argc) != 0)
			{
				ErrorPrint("ScriptObject::Call %s", lua_tostring(L, -1));
			}
		}
		LuaEngine::Restore();
	}

	int ScriptObject::ENGINE_NEW_SCRIPTOBJECT(lua_State * L)
	{
		if (!lua_istable(L, -1))
		{
			ErrorPrint("ScriptObject New: arg at -1 is not a lua table %d type %d", lua_gettop(L), lua_type(L, -1));
			lua_pushnil(L);
			return 1;
		}

		ScriptObject** pObj = (ScriptObject**)lua_newuserdata(L, sizeof(ScriptObject*));
		ScriptObject* pMemory = new (kMemDefaultId)ScriptObject;
		*pObj = pMemory;
		SET_LUA_TYPE(L, ScriptObject);
		lua_setfield(L, -2, "ScriptObject");
		pMemory->luaData_ = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_getref(L, pMemory->luaData_);
		pMemory->OnInitScript();
		return 1;
	}

	void ScriptObject::CleanUpScripts(lua_State * L)
	{
		LuaEngine::Store();
		lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
		lua_pushnil(L);
		lua_setfield(L, -2, "Assets");
		LuaEngine::Restore();
	}
}