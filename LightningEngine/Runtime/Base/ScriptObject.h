#pragma once

#include "Component.h"
#include "LuaEngine/LuaCallback.h"


namespace LightningEngine
{
	class GameObject;
	class TouchEvent;
	class MouseWheelEvent;

	class ScriptObject : public Component
	{
	public:
		DECLEAR_ENGINE_CLASS(ScriptObject)
		ScriptObject();
		~ScriptObject();
		static int Export(lua_State* L);
		static int DefaultGC(lua_State* L);
		void Awake(lua_State* L);
		void OnInitScript();
		void OnDestroyScript();
		bool GetMethod(lua_State* L, const char* methodName);
		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void OnTouchBegin(const TouchEvent& touch);
		void OnTouchEnd(const TouchEvent& touch);
		void OnTouchCanceled(const TouchEvent& touch);
		void OnTouchMove(const TouchEvent& touch);
		void OnTouchEnter(const TouchEvent& touch);
		void OnTouchLeave(const TouchEvent& touch);
		void OnTouchWheel(const MouseWheelEvent& touch);
		void OnMouseWheel(const MouseWheelEvent* event);
		void OnCollideBegin(GameObject* obj);
		void OnCollideEnd(GameObject* obj);
		void OnDestroy();
		void OnShow();
		void OnHide();
		void OnViewportChange(float width, float height);
		void OnKeyUp(int code);
		void OnKeyDown(int code);
		void OnIMEChar(const char* utf8Str);
		void OnIMECompositionString(const char* utf8Str, bool bRet);
		void OnEditEnd();
		void OnChar(UInt32 charCode);
		void OnPaste(const char* str, int len);
		void OnIAP(void* product);
		void OnAdReward(void* product);
		void OnAnimationEnd(const char* animationName);
		void OnPostRendering(Object* colorBuffer);
		void Call(const char* method, Param* param);
		static int ENGINE_NEW_SCRIPTOBJECT(lua_State* L);
		static void CleanUpScripts(lua_State* L);

	public:
		int update_;
		int fixedUpdate_;
		int onChar_;
		int onIMEChar_;
		int onTouchBegin_;
		int onTouchEnd_;
		int onTouchCanceled_;
		int onTouchMove_;
		int onTouchEnter_;
		int onTouchLeave_;
		//int onTouchWheel_;
		int onMouseWheel_;
		int onKeyUp_;
		int onKeyDown_;
		int animationEnd_;
		int onPostRendering_;
	};
}