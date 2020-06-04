#pragma once
#include "Object.h"
#include "Utils/LinkedList.h"
#include "Allocator/TLSFAllocator.h"

#define UPDATER(deltaTime) void Update(float deltaTime)


namespace LightningEngine
{
	enum ComponentType
	{
		CT_Transform = 1,
		CT_Object2D = 2,
		CT_ComponentTypeCount
	};

	class Component : public Object, public DoubleLinkedList
	{
	public:
		DECLEAR_ENGINE_CLASS(Component)
		Component();
		virtual ~Component();
		virtual void Update(float deltaTime);
		virtual void Enable();
		virtual void Disable();
		virtual Component* Clone();
		void InitComponent(lua_State* L);
		bool IsEnable();
		static int Export(lua_State* L);
		DEFINE_LUA_API(GetClassID);
		DEFINE_LUA_API(Enable);
		DEFINE_LUA_API(GameObject);
		
	public:
		bool bEnable_;
		bool bPositionChanged_;
	};
}