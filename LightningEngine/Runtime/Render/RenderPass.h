#pragma once

#include "RuntimePrefix.h"
#include "Utils/SmartPtr.h"
#include "Shader.h"
#include "RenderMisc.h"
#include "RenderState.h"
#include "MaterialProperty.h"
#include "UniformUpdater.h"

namespace LightningEngine
{
	class Material;
	
	class RenderPass : public Object
	{
	public:
		RenderPass();
		virtual ~RenderPass();
		void operator=(const RenderPass& pass);
		RenderPass* Clone()const;
		void SetShader(Shader* shader);
		void ActiveUniform(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformFloat(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformVec2(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformVec3(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformVec4(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformMat3(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformMat4(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformSampler2D(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformSamplerCube(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformIntArray(const char* name, const ShaderUniformDefine* def);
		void ActiveUniformVec4Array(const char* name, const ShaderUniformDefine* def);
		TextureUnitProperty* GetTextureProperty(const char* uniformName);
		UniformMatri4x4Property* GetMatrix4Property(const char* uniformName);
		UniformVector4Property* GetVec4Property(const char* uniformName);
		void SetTextureProperty(const char* uniformName, TextureUnit* texture);
		void SetTextureCubeProperty(const char* uniformName, TextureCube* texture);
		void SetFloatProperty(const char* uniformName, float v);
		void SetVec2Property(const char* uniformName, float x, float y);
		void SetVec3Property(const char* uniformName, float x, float y, float z);
		void SetVec4Property(const char* uniformName, float x, float y, float z, float w);
		void SetVec4Property(const char* uniformName, float* val);
		void SetVec4Property(const char* uniformName, const Serializer::Vector4f& val);
		void SetVec4ArrayProperty(const char* uniformName, int size, float* val);
		void SetIntArrayProperty(const char* uniformName, int size, int* val);
		void SetMatrix4Property(const char* uniformName, const float* val);
		void SetMatrix3Property(const char* uniformName, float* val);
		void UpdateUniforms(void* pCamera);
		void ClearUniformUpdaters();
		void AddUniformUpdater(UniformUpdater* pUpdater);
		void ActiveRenderState();
		void SetupStencilBuffer();
		void RestoreStencilBuffer();
		DECLEAR_ENGINE_CLASS(RenderPass)

	public:
		RenderState renderState_;
		bool bWriteStencilMask_;
		bool bUseStencilMask_;
		RenderPassCatagory catagory_;
		std::unordered_map<std::string, MaterialProperty*> properties_;
		std::unordered_map<std::string, UniformUpdater*> propertyUpdaters_;
		std::set<UniformUpdater*> activeUniformList_;
		SmartPtr<Shader> shader_;
	};
}