#include "UniformUpdater.h"
#include "MaterialProperty.h"
#include "Render/RenderState.h"
#include "Debugger/Debugger.h"
#include "Camera.h"

namespace LightningEngine
{
	UniformUpdater::UniformUpdater(SetUniformFunc func, GLint location, void * userData) : 
		location_(location), 
		func_(func), 
		userData_(userData)
	{
	}

	UniformUpdater::UniformUpdater(UniformUpdater * pUniform) : 
		location_(pUniform->location_), 
		func_(pUniform->func_), 
		userData_(pUniform->userData_)
	{
	}

	UniformUpdater::~UniformUpdater()
	{
	}

	void UniformUpdater::Update(void * param)
	{
		if (nullptr != func_)
		{
			func_(location_, param, userData_);
		}

		if (nullptr != next_)
		{
			Next<UniformUpdater>()->Update(param);
		}
	}

	void UniformUpdater::operator=(UniformUpdater & uniform)
	{
		func_ = uniform.func_;
		location_ = uniform.location_;
		userData_ = uniform.userData_;
	}

	void UniformUpdater::UpdateTextureUnit(GLint loc, void * camera, void * userData)
	{
		if (nullptr != userData)
		{
			TextureUnitProperty* pTexPro = (TextureUnitProperty*)userData;
			if (pTexPro->value_ != nullptr)
			{
				OGL_CALL(glActiveTexture(GL_TEXTURE0 + pTexPro->slot_));
				OGL_CALL(glBindTexture(pTexPro->value_->textureType_,
					pTexPro->value_->textureID_));
				OGL_CALL(glUniform1i(loc, pTexPro->slot_));
			}
		}
	}

	void UniformUpdater::UpdateProjectionMatrix(GLint loc, void * camera, void * userData)
	{
		Camera* pCamera = (Camera*)camera;
		OGL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, pCamera->projectionMatrix_.data_));
	}

	void UniformUpdater::UpdateViewMatrix(GLint loc, void * camera, void * userData)
	{
		Camera* pCamera = (Camera*)camera;
		OGL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, pCamera->viewMatrix_.data_));
	}

	void UniformUpdater::UpdateFloat(GLint loc, void * camera, void * userData)
	{
		UniformFloatProperty* floatPro = (UniformFloatProperty*)userData;
		OGL_CALL(glUniform1fv(loc, 1, &floatPro->value_));
	}

	void UniformUpdater::UpdateVec2(GLint loc, void * camera, void * userData)
	{
		UniformVector2Property* vec2Pro = (UniformVector2Property*)userData;
		OGL_CALL(glUniform2fv(loc, 1, vec2Pro->value_.v_));
	}

	void UniformUpdater::UpdateVec3(GLint loc, void * camera, void * userData)
	{
		UniformVector3Property* vec3Pro = (UniformVector3Property*)userData;
		OGL_CALL(glUniform3fv(loc, 1, vec3Pro->value_.v_));
	}

	void UniformUpdater::UpdateVec4(GLint loc, void * camera, void * userData)
	{
		UniformVector4Property* vec4Pro = (UniformVector4Property*)userData;
		OGL_CALL(glUniform4fv(loc, 1, vec4Pro->value_.v_));
	}

	void UniformUpdater::UpdateMat3(GLint loc, void * camera, void * userData)
	{
		UniformMatri3x3Property* mat3Pro = (UniformMatri3x3Property*)userData;
		OGL_CALL(glUniformMatrix3fv(loc, 1, GL_FALSE, mat3Pro->value_.data_));
	}

	void UniformUpdater::UpdateMat4(GLint loc, void * camera, void * userData)
	{
		UniformMatri4x4Property* mat4Pro = (UniformMatri4x4Property*)userData;
		OGL_CALL(glUniformMatrix4fv(loc, 1, GL_FALSE, mat4Pro->value_.data_));
	}

	void UniformUpdater::UpdateIntArray(GLint loc, void * camera, void * userData)
	{
		UniformIntArrayProperty* intArrPro = (UniformIntArrayProperty*)userData;
		if (intArrPro->size_ == 0)
		{
			return;
		}

		OGL_CALL(glUniform1iv(loc, intArrPro->size_, intArrPro->value_));
	}

	void UniformUpdater::UpdateVec4Array(GLint loc, void * camera, void * userData)
	{
		UniformVector4ArrayProperty* vec4ArrPro = (UniformVector4ArrayProperty*)userData;
		if (vec4ArrPro->size_ == 0)
		{
			return;
		}

		OGL_CALL(glUniform4fv(loc, vec4ArrPro->size_, vec4ArrPro->value_));
	}
}
