#pragma once
#include "Object.h"
#include "Utils/LinkedList.h"
#include "Allocator/TLSFAllocator.h"

#define UPDATER(deltaTime) void Update(float deltaTime)

#if LIGHTNING_ENGINE_PLATFORM_WIN
#define IMP_RENDER(t) void t::Render(RenderQueue* rq)
#define RENDER(dc) void Render(RenderQueue* rq)
#else
#endif

namespace LightningEngine
{
	class GameObject;
	class DrawCall;
	class RenderQueue;

	enum ComponentType
	{
		CT_Transform = 1,
		CT_Object2D = 2,
		CT_ImageSprite = 3,
		CT_ImageSprite9 = 4,
		CT_AudioSource = 5,
		CT_DynamicFont = 6,
		CT_Label = 7,
		CT_ComponentTypeCount
	};

	class Component : public Object, public DoubleLinkedList
	{
	public:
		DECLEAR_ENGINE_CLASS(Component)
		Component();
		virtual ~Component();
		void SetOwner(GameObject* go, bool is_script = false);
		void* PrepareUpdateMaterial(void* mat);
		virtual void Update(float deltaTime);
		virtual void Render(RenderQueue* rq);
		virtual void RenderShadowMap(RenderQueue* rq);
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
		GameObject* owner_;

	};
}