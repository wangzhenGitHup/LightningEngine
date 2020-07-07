#include "RenderPass.h"
#include "RenderState.h"
#include "Serializer/Vector.serializer.h"

namespace LightningEngine
{
	RenderPass::RenderPass() : Object(1),
		shader_(nullptr),
		bUseStencilMask_(false),
		bWriteStencilMask_(false),
		catagory_(kRenderPassCatagoryOnePass)
	{
	}

	RenderPass::~RenderPass()
	{
		ClearUniformUpdaters();
	}

	void RenderPass::operator=(const RenderPass & pass)
	{
		shader_ = pass.shader_.ptr_;
	}

	RenderPass * RenderPass::Clone() const
	{
		RenderPass* pass = new RenderPass;
		pass->shader_ = shader_.ptr_;
		pass->bUseStencilMask_ = bUseStencilMask_;
		pass->bWriteStencilMask_ = bWriteStencilMask_;
		pass->catagory_ = catagory_;
		pass->renderState_ = renderState_;

		for (auto iter = properties_.begin(); iter != properties_.end(); ++iter)
		{
			pass->properties_.insert(std::pair<std::string, MaterialProperty*>(iter->first, iter->second->Clone()));
		}

		for (auto iter = propertyUpdaters_.begin(); iter != propertyUpdaters_.end(); ++iter)
		{
			UniformUpdater* src = iter->second;
			UniformUpdater* updater = new UniformUpdater(src);
			updater->userData_ = pass->properties_[iter->first];
			pass->AddUniformUpdater(updater);
			pass->propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(iter->first, updater));
		}
		return pass;
	}

	void RenderPass::SetShader(Shader * shader)
	{
		shader_ = shader;
		ClearUniformUpdaters();
		for (auto iter = shader_->activeUniforms_.begin(); iter != shader_->activeUniforms_.end(); ++iter)
		{
			ActiveUniform(iter->first.c_str(), iter->second);
		}

		auto iter = properties_.find("U_Color");
		if (iter != properties_.end())
		{
			((UniformVector4Property*)(iter->second))->value_.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		iter = properties_.find(UniformDefines::Vec4_ColorScale_NameInShader);
		if (iter != properties_.end())
		{
			((UniformVector4Property*)(iter->second))->value_.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	void RenderPass::ActiveUniform(const char * name, const ShaderUniformDefine * def)
	{
		switch (def->type_)
		{
		case GL_FLOAT:
			ActiveUniformFloat(name, def);
			break;

		case GL_FLOAT_VEC2:
			ActiveUniformVec2(name, def);
			break;

		case GL_FLOAT_VEC3:
			ActiveUniformVec3(name, def);
			break;

		case GL_FLOAT_VEC4:
			if (def->size_ == 1)
			{
				ActiveUniformVec4(name, def);
			}
			else
			{
				ActiveUniformVec4Array(name, def);
			}
			break;

		case GL_FLOAT_MAT3:
			ActiveUniformMat3(name, def);
			break;

		case GL_FLOAT_MAT4:
			ActiveUniformMat4(name, def);
			break;

		case GL_INT:
			if (def->size_ == 1)
			{

			}
			else
			{
				ActiveUniformIntArray(name, def);
			}
			break;

		case GL_SAMPLER_2D:
			ActiveUniformSampler2D(name, def);
			break;

		case GL_SAMPLER_CUBE:
			ActiveUniformSamplerCube(name, def);
			break;

		}
	}

	void RenderPass::ActiveUniformFloat(const char * name, const ShaderUniformDefine * def)
	{
		auto iter = properties_.find(name);
		MaterialProperty* materialPro = nullptr;
		if (iter == properties_.end())
		{
			UniformFloatProperty* prop = new(kMemMaterialId)UniformFloatProperty();
			prop->location_ = def->location_;
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			materialPro = prop;
		}
		else
		{
			materialPro = iter->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateFloat, 
				def->location_, materialPro);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	void RenderPass::ActiveUniformVec2(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* materilaPro = nullptr;
		if (iter_prop == properties_.end())
		{
			UniformVector2Property* prop = new(kMemMaterialId)UniformVector2Property();
			prop->location_ = def->location_;
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			materilaPro = prop;
		}
		else
		{
			materilaPro = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateVec2, 
				def->location_, materilaPro);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	void RenderPass::ActiveUniformVec3(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* materialProp = nullptr;
		if (iter_prop == properties_.end())
		{
			UniformVector3Property* prop = new(kMemMaterialId)UniformVector3Property();
			prop->location_ = def->location_;
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			materialProp = prop;
		}
		else
		{
			materialProp = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateVec3, 
				def->location_, materialProp);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	void RenderPass::ActiveUniformVec4(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* materialProp = nullptr;
		if (iter_prop == properties_.end())
		{
			UniformVector4Property* prop = new(kMemMaterialId)UniformVector4Property();
			prop->location_ = def->location_;
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			materialProp = prop;
		}
		else
		{
			materialProp = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateVec4, def->location_, materialProp);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	void RenderPass::ActiveUniformMat3(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* material_prop = nullptr;
		if (iter_prop == properties_.end())
		{
			UniformMatri3x3Property* prop = new(kMemMaterialId)UniformMatri3x3Property();
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			prop->location_ = def->location_;
			material_prop = prop;
		}
		else
		{
			material_prop = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateMat3, def->location_, material_prop);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
	}

	void RenderPass::ActiveUniformMat4(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* material_prop = nullptr;
		if (iter_prop == properties_.end())
		{
			UniformMatri4x4Property* prop = new(kMemMaterialId)UniformMatri4x4Property();
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			prop->location_ = def->location_;
			material_prop = prop;
		}
		else
		{
			material_prop = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = nullptr;
			if (strcmp(name, UniformDefines::Matrix_P_NameInShader) == 0)
			{
				updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateProjectionMatrix, def->location_, material_prop);
			}
			else if (strcmp(name, UniformDefines::Matrix_V_NameInShader) == 0)
			{
				updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateViewMatrix, def->location_, material_prop);
			}
			else
			{
				updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateMat4, def->location_, material_prop);
			}

			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	void RenderPass::ActiveUniformSampler2D(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* material_prop = nullptr;
		if (iter_prop == properties_.end())
		{
			TextureUnitProperty* prop = new(kMemMaterialId)TextureUnitProperty(kUniformTypeSampler2D);
			prop->name_ = name;
			prop->location_ = def->location_;
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_prop = prop;
		}
		else
		{
			material_prop = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateTextureUnit, def->location_, material_prop);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	void RenderPass::ActiveUniformSamplerCube(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* material_prop = nullptr;
		if (iter_prop == properties_.end())
		{
			TextureUnitProperty* prop = new(kMemMaterialId)TextureUnitProperty(kUniformTypeSamplerCube);
			prop->name_ = name;
			prop->location_ = def->location_;
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_prop = prop;
		}
		else
		{
			material_prop = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateTextureUnit, def->location_, material_prop);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	void RenderPass::ActiveUniformIntArray(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* material_prop = nullptr;
		if (iter_prop == properties_.end())
		{
			UniformIntArrayProperty* prop = new(kMemMaterialId)UniformIntArrayProperty();
			prop->value_ = nullptr;
			prop->location_ = def->location_;
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_prop = prop;
		}
		else
		{
			material_prop = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateIntArray, def->location_, material_prop);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	void RenderPass::ActiveUniformVec4Array(const char * name, const ShaderUniformDefine * def)
	{
		auto iter_prop = properties_.find(name);
		MaterialProperty* material_prop = nullptr;
		if (iter_prop == properties_.end())
		{
			UniformVector4ArrayProperty* prop = new(kMemMaterialId)UniformVector4ArrayProperty();
			prop->value_ = nullptr;
			prop->location_ = def->location_;
			properties_.insert(std::pair<std::string, MaterialProperty*>(name, prop));
			material_prop = prop;
		}
		else
		{
			material_prop = iter_prop->second;
		}

		auto iter_updater = propertyUpdaters_.find(name);
		if (iter_updater == propertyUpdaters_.end())
		{
			UniformUpdater* updater = new(kMemMaterialId)UniformUpdater(UniformUpdater::UpdateVec4Array, def->location_, material_prop);
			propertyUpdaters_.insert(std::pair<std::string, UniformUpdater*>(name, updater));
			AddUniformUpdater(updater);
		}
		else
		{
			AddUniformUpdater(iter_updater->second);
		}
	}

	TextureUnitProperty * RenderPass::GetTextureProperty(const char * uniformName)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeSampler2D)
			{
				return (TextureUnitProperty*)iter->second;
			}
		}
		return nullptr;
	}

	UniformMatri4x4Property * RenderPass::GetMatrix4Property(const char * uniformName)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeMatri4x4)
			{
				return (UniformMatri4x4Property*)iter->second;
			}
		}
		return nullptr;
	}

	UniformVector4Property * RenderPass::GetVec4Property(const char * uniformName)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeVec4)
			{
				return(UniformVector4Property*)iter->second;
			}
		}
		return nullptr;
	}

	void RenderPass::SetTextureProperty(const char * uniformName, TextureUnit * texture)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeSampler2D)
			{
				((TextureUnitProperty*)iter->second)->value_ = texture;
			}
		}
	}

	void RenderPass::SetTextureCubeProperty(const char * uniformName, TextureCube * texture)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeSamplerCube)
			{
				((TextureUnitProperty*)iter->second)->value_ = texture;
			}
		}
	}

	void RenderPass::SetFloatProperty(const char * uniformName, float v)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeFloat)
			{
				((UniformFloatProperty*)iter->second)->value_ = v;
			}
		}
	}

	void RenderPass::SetVec2Property(const char * uniformName, float x, float y)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeVec2)
			{
				((UniformVector2Property*)iter->second)->value_.Set(x, y);
			}
		}
	}

	void RenderPass::SetVec3Property(const char * uniformName, float x, float y, float z)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeVec3)
			{
				((UniformVector3Property*)iter->second)->value_.Set(x, y, z);
			}
		}
	}

	void RenderPass::SetVec4Property(const char * uniformName, float x, float y, float z, float w)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeVec4)
			{
				((UniformVector4Property*)iter->second)->value_.Set(x, y, z, w);
			}
		}
	}

	void RenderPass::SetVec4Property(const char * uniformName, float * val)
	{
		SetVec4Property(uniformName, val[0], val[1], val[2], val[3]);
	}

	void RenderPass::SetVec4Property(const char * uniformName, const Serializer::Vector4f & val)
	{
		SetVec4Property(uniformName, val.x(), val.y(), val.z(), val.w());
	}

	void RenderPass::SetVec4ArrayProperty(const char * uniformName, int size, float * val)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeVector4Array)
			{
				((UniformVector4ArrayProperty*)iter->second)->size_ = size;
				((UniformVector4ArrayProperty*)iter->second)->value_ = val;
			}
		}
	}

	void RenderPass::SetIntArrayProperty(const char * uniformName, int size, int * val)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeVector4Array)
			{
				((UniformIntArrayProperty*)iter->second)->size_ = size;
				((UniformIntArrayProperty*)iter->second)->value_ = val;
			}
		}
	}

	void RenderPass::SetMatrix4Property(const char * uniformName, const float * val)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeMatri4x4)
			{
				memcpy(((UniformMatri4x4Property*)iter->second)->value_.data_, val, sizeof(float) * 16);
			}
		}
	}

	void RenderPass::SetMatrix3Property(const char * uniformName, float * val)
	{
		auto iter = properties_.find(uniformName);
		if (iter != properties_.end())
		{
			if (iter->second->valueType_ == kUniformTypeMatri3x3)
			{
				memcpy(((UniformMatri3x3Property*)iter->second)->value_.data_, val, sizeof(float) * 9);
			}
		}
	}

	void RenderPass::UpdateUniforms(void * pCamera)
	{
		int iTextureSlot = 0;
		for (auto iter = activeUniformList_.begin(); iter != activeUniformList_.end(); ++iter)
		{
			UniformUpdater* updater = *iter;
			if (updater->func_ != nullptr)
			{
				if (updater->func_ == UniformUpdater::UpdateTextureUnit)
				{
					TextureUnitProperty* prop = (TextureUnitProperty*)updater->userData_;
					prop->slot_ = iTextureSlot++;
				}
				updater->func_(updater->location_, pCamera, updater->userData_);
			}
		}
	}

	void RenderPass::ClearUniformUpdaters()
	{
		if (!properties_.empty())
		{
			for (auto iter = properties_.begin(); iter != properties_.end(); ++iter)
			{
				delete iter->second;
			}
		}

		if (!propertyUpdaters_.empty())
		{
			for (auto iter = propertyUpdaters_.begin(); iter != propertyUpdaters_.end(); ++iter)
			{
				delete iter->second;
			}
		}

		properties_.clear();
		propertyUpdaters_.clear();
		activeUniformList_.clear();
	}

	void RenderPass::AddUniformUpdater(UniformUpdater * pUpdater)
	{
		activeUniformList_.insert(pUpdater);
	}

	void RenderPass::ActiveRenderState()
	{
		shader_->gpuProgram_->MakeAsCurrentShader();
		GlobalRenderState::EnableBlend(renderState_.bBlend_);
		if (renderState_.bBlend_)
		{
			GlobalRenderState::BlendFunc(renderState_.srcBlendFunc_, renderState_.dstBlendFunc_);
		}

		GlobalRenderState::EnableCull(renderState_.bCull_);
		GlobalRenderState::CullFace(renderState_.cullFace_);
		GlobalRenderState::EnableZWrite(renderState_.bWriteZ_);
		GlobalRenderState::EnableDepthTest(renderState_.bZTest_);
		GlobalRenderState::ZTest(renderState_.zTestFunction_);
		GlobalRenderState::ZOffset(renderState_.offsetFactor_, renderState_.offsetUnit_);
		GlobalRenderState::EnableAlphaTest(renderState_.bAlphaTest_);

		if (renderState_.bAlphaTest_)
		{
			GlobalRenderState::AlphaTest(renderState_.alphaTestFunction_, renderState_.alphaTestValue_);
		}

		GlobalRenderState::EnablePointSprite(renderState_.bEnablePointSprite_);
		GlobalRenderState::PolygonMode(renderState_.polygonMode_);
	}


	void RenderPass::SetupStencilBuffer()
	{
		if (bWriteStencilMask_)
		{
			GlobalRenderState::ClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glColorMask(false, false, false, false);
		}
		else if (bUseStencilMask_)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_EQUAL, 1, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
	}

	void RenderPass::RestoreStencilBuffer()
	{
		if (bWriteStencilMask_)
		{
			glDisable(GL_STENCIL_TEST);
			glColorMask(true, true, true, true);
		}
		else if (bUseStencilMask_)
		{
			glDisable(GL_STENCIL_TEST);
		}
	}
}