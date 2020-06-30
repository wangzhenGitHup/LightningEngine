#pragma once

#include "Utils/LinkedList.h"
#include "Render/LightningEngineGL.h"

namespace LightningEngine
{
	typedef void(*SetUniformFunc)(GLint location, void* camera, void* userData);

	class UniformUpdater : public LinkedList
	{
	public:
		UniformUpdater(SetUniformFunc func, GLint location, void* userData);
		UniformUpdater(UniformUpdater* pUniform);
		~UniformUpdater();
		void Update(void* param);
		void operator=(UniformUpdater& uniform);
		static void UpdateTextureUnit(GLint loc, void* camera, void* userData);
		static void UpdateProjectionMatrix(GLint loc, void* camera, void* userData);
		static void UpdateViewMatrix(GLint loc, void* camera, void* userData);
		static void UpdateFloat(GLint loc, void* camera, void* userData);
		static void UpdateVec2(GLint loc, void* camera, void* userData);
		static void UpdateVec3(GLint loc, void* camera, void* userData);
		static void UpdateVec4(GLint loc, void* camera, void* userData);
		static void UpdateMat3(GLint loc, void* camera, void* userData);
		static void UpdateMat4(GLint loc, void* camera, void* userData);
		static void UpdateIntArray(GLint loc, void* camera, void* userData);
		static void UpdateVec4Array(GLint loc, void* camera, void* userData);

		GLint location_;
		SetUniformFunc func_;
		void* userData_;
	};
}