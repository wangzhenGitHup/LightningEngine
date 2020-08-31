#include "Component.h"
#include "Scene/GameObject.h"
#include "Render/Material.h"

namespace LightningEngine
{
	Component::Component() : owner_(nullptr),
		bPositionChanged_(true), 
		bEnable_(true)
	{
	}

	Component::~Component()
	{
	}

	void Component::SetOwner(GameObject * go, bool bScript)
	{
		owner_ = go;
		if (bScript)
		{
			if (!owner_->scripts_)
			{
				owner_->scripts_ = (ScriptObject*)this;
			}
			else
			{
				owner_->scripts_->Append(this);
			}
		}
		else
		{
			go->AddComponent(this);
		}
	}

	void * Component::PrepareUpdateMaterial(void * mat)
	{
		Material* pMaterial = (Material*)mat;
		if (pMaterial->bSharedMaterial_)
		{
			Material* pRet = new Material;
			*pRet = *pMaterial;
			pMaterial->release();
			pMaterial = pRet;
			pMaterial->bSharedMaterial_ = false;
			pMaterial->retain();
		}

		return pMaterial;
	}

	void Component::Update(float deltaTime)
	{
	}

	void Component::Render(RenderQueue * rq)
	{
	}

	void Component::RenderShadowMap(RenderQueue * rq)
	{
	}

	void Component::Enable()
	{
	}

	void Component::Disable()
	{
		if (!bEnable_)
		{
			return;
		}

		bEnable_ = true;
	}

	bool Component::IsEnable()
	{
		return bEnable_ && owner_->IsEnabled();
	}

	int Component::Export(lua_State * L)
	{
		luaL_Reg api_m[] = {
			EXPORT_API(GameObject)
			EXPORT_API(Enable)
			EXPORT_API(GetClassID)
			{NULL, NULL}
		};

		NEW_LUA_TYPE_WITH_API(L, Component, api_m);
		return 0;
	}

	int Component::API_GetClassID(lua_State* L)
	{
		Component* component = TO_USERDATA(L, Component, 1);
		if (lua_toboolean(L, 2))
		{
			component->Enable();
		}
		else
		{
			component->Disable();
		}

		return 0;
	}

	int Component::API_GameObject(lua_State* L)
	{
		Component* component = TO_USERDATA(L, Component, 1);
		if (component->owner_)
		{
			component->owner_->PushUserData(L);
		}

		return 1;
	}

	void Component::InitComponent(lua_State* L)
	{
		luaData_ = luaL_ref(L, LUA_REGISTRYINDEX);
		lua_getref(L, luaData_);
	}

	Component * Component::Clone()
	{
		return nullptr;
	}
}