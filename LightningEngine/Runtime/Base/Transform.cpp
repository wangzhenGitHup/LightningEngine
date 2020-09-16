#include "Transform.h"
#include "Debugger/Log.h"

namespace LightningEngine
{
	Transform::Transform() : type_(kTransformTypeNormal),
		bAffectedByPhysics_(false),
		bDirty_(true)
	{
		positionVec_.Set(0.0f, 0.0f, 0.0f);
		rotationVec_.Set(0.0f, 0.0f, 0.0f);
		localQuaternion_.Set(0.0f, 0.0f, 0.0f, 1.0f);
		scaleVec_.Set(1.0f, 1.0f, 1.0f);
	}

	void Transform::SetLocalPosition(float x, float y, float z, bool affectByPhysics)
	{
		positionVec_.Set(x, y, z);
		bDirty_ = true;
		if (owner_ != nullptr)
		{
			owner_->MarkWorldMatrixDirty();
			owner_->bLocalMatrixDirty = true;
		}

		bAffectedByPhysics_ = affectByPhysics;
	}

	void Transform::SetLocalRotation(float x, float y, float z, bool affectByPhysics)
	{
		rotationVec_.Set(x, y, z);
		localQuaternion_ = EulerToQuaternion(rotationVec_ * ENGINE_PI_DIV_180);
		bDirty_ = true;

		if (nullptr != owner_)
		{
			owner_->MarkWorldMatrixDirty();
			owner_->bLocalMatrixDirty = true;
		}

		bAffectedByPhysics_ = affectByPhysics;
	}

	void Transform::SetLocalScale(float x, float y, float z)
	{
		scaleVec_.Set(x, y, z);
		bDirty_ = true;
		if (nullptr != owner_)
		{
			owner_->MarkWorldMatrixDirty();
			owner_->bLocalMatrixDirty = true;
		}
	}

	void Transform::SetLocalShearing(float x, float y, float z)
	{
		shearingVec_.Set(x, y, z);
		bDirty_ = true;
		if (nullptr != owner_)
		{
			owner_->MarkWorldMatrixDirty();
			owner_->bLocalMatrixDirty = true;
		}
	}

	void Transform::ToMatrix(Matrix4x4 & mat)
	{
		Matrix3x3 rotationMat;
		if (FLT_IS_ZERO(shearingVec_.x_) && FLT_IS_ZERO(shearingVec_.y_))
		{
			QuaternionToMatrix(localQuaternion_, rotationMat);
		}
		else
		{
			Matrix3x3 shearingMat(1.0f);
			shearingMat.data_[1] = ENGINE_DEG2RAD(shearingVec_.x_);
			shearingMat.data_[3] = ENGINE_DEG2RAD(-shearingVec_.y_);
			QuaternionToMatrix(localQuaternion_, rotationMat);
			rotationMat = rotationMat * shearingMat;
		}

		mat.LoadIdentity();
		mat.SetTranslatePart(positionVec_.x_, positionVec_.y_, positionVec_.z_);
		mat.SetRotationPart(rotationMat.data_);
		mat.Scale(scaleVec_.x_, scaleVec_.y_, scaleVec_.z_);
	}

	void Transform::operator=(const Transform * trans)
	{
		positionVec_.Set(trans->positionVec_.v_);
		scaleVec_.Set(trans->scaleVec_.v_);
		rotationVec_.Set(trans->rotationVec_.v_);
		shearingVec_.Set(trans->shearingVec_.v_);
		bDirty_ = true;
		if (nullptr != owner_)
		{
			owner_->MarkWorldMatrixDirty();
			owner_->bLocalMatrixDirty = true;
		}
	}

	void Transform::operator=(const Transform& trans)
	{
		positionVec_.Set(trans.positionVec_.v_);
		scaleVec_.Set(trans.scaleVec_.v_);
		rotationVec_.Set(trans.rotationVec_.v_);
		shearingVec_.Set(trans.shearingVec_.v_);
		bDirty_ = true;
		if (nullptr != owner_)
		{
			owner_->MarkWorldMatrixDirty();
			owner_->bLocalMatrixDirty = true;
		}
	}

	void Transform::Dump(int lv)
	{
		InfoPrint("local position: %f, %f, %f", positionVec_.x_, positionVec_.y_, positionVec_.z_);
		InfoPrint("local rotation: %f, %f, %f", rotationVec_.x_, rotationVec_.y_, rotationVec_.z_);
		InfoPrint("local scale: %f, %f, %f", scaleVec_.x_, scaleVec_.y_, scaleVec_.z_);
		InfoPrint("local shearing: %f, %f, %f", shearingVec_.x_, shearingVec_.y_, shearingVec_.z_);
	}

	int Transform::Export(lua_State* L)
	{
		luaL_Reg api_m[] = {
			EXPORT_API(SetPosition)
			EXPORT_API(GetPosition)
			EXPORT_API(SetRotation)
			EXPORT_API(GetRotation)
			EXPORT_API(SetScale)
			EXPORT_API(GetScale)
			EXPORT_API(SetShearing)
			EXPORT_API(GetShearing)
			EXPORT_API_END
		};

		NEW_LUA_TYPE_WITH_API(L, Transform, api_m);
		SET_PARENT(L, Component);

		return 0;
	}

	int Transform::API_SetPosition(lua_State* L)
	{
		if (lua_gettop(L) != 4)
		{
			ErrorPrint("Transform::SetPosition arg count must be 3, current is %d", lua_gettop(L) - 1);
			return 0;
		}

		if (!lua_isnumber(L, 2) ||
			!lua_isnumber(L, 3) ||
			!lua_isnumber(L, 4))
		{
			ErrorPrint("Transform::SetPosition arg must be number, current 1(%s), 2(%s), 3(%s)",
				LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}

		Transform* ptr = TO_USERDATA(L, Transform, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->SetLocalPosition(x, y, z);
		return 0;
	}

	int Transform::API_GetPosition(lua_State* L)
	{
		Transform* ptr = TO_USERDATA(L, Transform, 1);
		lua_pushnumber(L, ptr->positionVec_.x_);
		lua_pushnumber(L, ptr->positionVec_.y_);
		lua_pushnumber(L, ptr->positionVec_.z_);

		return 3;
	}

	int Transform::API_SetRotation(lua_State*L)
	{
		if (lua_gettop(L) != 4)
		{
			ErrorPrint("Transform::SetRotation arg count must be 3, current is %d", lua_gettop(L) - 1);
			return 0;
		}

		if (!lua_isnumber(L, 2) ||
			!lua_isnumber(L, 3) ||
			!lua_isnumber(L, 4))
		{
			ErrorPrint("Transform::SetRotation arg must be number, current 1(%s), 2(%s), 3(%s)",
				LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}

		Transform* ptr = TO_USERDATA(L, Transform, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->SetLocalRotation(x, y, z);
		return 0;
	}

	int Transform::API_GetRotation(lua_State*L)
	{
		Transform* ptr = TO_USERDATA(L, Transform, 1);
		lua_pushnumber(L, ptr->rotationVec_.x_);
		lua_pushnumber(L, ptr->rotationVec_.y_);
		lua_pushnumber(L, ptr->rotationVec_.z_);
		return 3;
	}

	int Transform::API_SetScale(lua_State*L)
	{
		if (lua_gettop(L) != 4)
		{
			ErrorPrint("Transform::SetScale arg count must be 3, current is %d", lua_gettop(L) - 1);
			return 0;
		}

		if (!lua_isnumber(L, 2) ||
			!lua_isnumber(L, 3) ||
			!lua_isnumber(L, 4))
		{
			ErrorPrint("Transform::SetScale arg must be number, current 1(%s), 2(%s), 3(%s)",
				LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}

		Transform* ptr = TO_USERDATA(L, Transform, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->SetLocalScale(x, y, z);
		return 0;
	}

	int Transform::API_GetScale(lua_State*L)
	{
		Transform* ptr = TO_USERDATA(L, Transform, 1);
		lua_pushnumber(L, ptr->scaleVec_.x_);
		lua_pushnumber(L, ptr->scaleVec_.y_);
		lua_pushnumber(L, ptr->scaleVec_.z_);
		return 3;
	}

	int Transform::API_SetShearing(lua_State*L)
	{
		if (lua_gettop(L) != 4)
		{
			ErrorPrint("Transform::SetShearing arg count must be 3, current is %d",
				lua_gettop(L) - 1);
			return 0;
		}

		if (!lua_isnumber(L, 2) ||
			!lua_isnumber(L, 3) ||
			!lua_isnumber(L, 4))
		{
			ErrorPrint("Transform::SetShearing arg must be number, current 1(%s), 2(%s), 3(%s)",
				LUA_TYPE(L, 2), LUA_TYPE(L, 3), LUA_TYPE(L, 4));
			return 0;
		}

		Transform* ptr = TO_USERDATA(L, Transform, 1);
		float x = lua_tonumber(L, 2);
		float y = lua_tonumber(L, 3);
		float z = lua_tonumber(L, 4);
		ptr->SetLocalShearing(x, y, z);
		return 0;
	}

	int Transform::API_GetShearing(lua_State*L)
	{
		Transform* ptr = TO_USERDATA(L, Transform, 1);
		lua_pushnumber(L, ptr->shearingVec_.x_);
		lua_pushnumber(L, ptr->shearingVec_.y_);
		lua_pushnumber(L, ptr->shearingVec_.z_);
		return 3;
	}
}