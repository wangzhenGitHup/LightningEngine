#pragma once

#include "Component.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Math/Matrix4x4.h"

namespace LightningEngine
{
	enum TransformType
	{
		kTransformTypeNormal,
		kTransformTypeOnlyTranslation,
		kTransformTypeNoRotationOrReflection,
		kTransformTypeNoScale,
		kTransformTypeNoScaleOrReflection,
		kTransformTypeCount
	};

	class Transform : public Component
	{
	public:
		Transform();
		void SetLocalPosition(float x, float y, float z, bool affectByPhysics = false);
		void SetLocalRotation(float x, float y, float z, bool affectByPhysics = false);
		void SetLocalScale(float x, float y, float z);
		void SetLocalShearing(float x, float y, float z);
		void ToMatrix(Matrix4x4& mat);
		void operator=(const Transform* trans);
		void operator=(const Transform& trans);
		void Dump(int lv = 0);

		static int Export(lua_State* L);
		/*int API_SetPosition(lua_State* L);
		int API_GetPosition(lua_State* L);
		int API_SetRotation(lua_State*L);
		int API_GetRotation(lua_State*L);
		int API_SetScale(lua_State*L);
		int API_GetScale(lua_State*L);
		int API_SetShearing(lua_State*L);
		int API_GetShearing(lua_State*L);*/

		DECLEAR_ENGINE_CLASS(Transform)

	private:
		DEFINE_LUA_API(SetPosition);
		DEFINE_LUA_API(GetPosition);
		DEFINE_LUA_API(SetRotation);
		DEFINE_LUA_API(GetRotation);
		DEFINE_LUA_API(SetScale);
		DEFINE_LUA_API(GetScale);
		DEFINE_LUA_API(SetShearing);
		DEFINE_LUA_API(GetShearing);

	public:
		TransformType type_;
		Vector3 scaleVec_;
		Vector3 positionVec_;
		Vector3 rotationVec_;
		Vector3 shearingVec_;
		Quaternion localQuaternion_;
		bool bDirty_;
		bool bAffectedByPhysics_;
	};
}