#include "Material.h"
#include "Camera.h"
#include "String/StringUtils.h"
#include "RenderState.h"

namespace LightningEngine
{
	Material* Material::s_default2DMaterial = nullptr;
	UniformVector4Property Material::time_;
	UniformVector4Property Material::sinTime_;
	UniformVector4Property Material::cosTime_;
	std::unordered_map<std::string, Material*> Material::s_cachedMaterials;

	Material::Material() : Object(1), 
		bSharedMaterial_(true),
		bCastShadow_(true),
		bReceiveShadow_(false),
		renderingQueue_(kRenderingQueueOpaque)
	{
	}

	Material::~Material()
	{
		CleanUpProperties();
	}

	void Material::SetStencilBuffer()
	{}

	void Material::RestoreStencilBuffer()
	{}

	void Material::EnableBlend(bool enable)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.bBlend_ = enable;
		}
	}

	void Material::EnableZTest(bool enable)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.bZTest_ = enable;
		}
	}

	void Material::EnableZWrite(bool enable)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.bWriteZ_ = enable;
		}
	}

	void Material::EnableCull(bool enable)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.bCull_ = enable;
		}
	}

	void Material::EnablePointSprite(bool enable)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.bEnablePointSprite_ = enable;
		}
	}

	void Material::EnableWriteStencilMask(bool enable)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->bWriteStencilMask_ = enable;
		}
	}

	void Material::EnableUseStencilMask(bool enable)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->bUseStencilMask_ = enable;
		}
	}

	void Material::ZTestFunction(int func)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.zTestFunction_ = func;
		}
	}

	void Material::ZOffset(float factor, float unit)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.offsetFactor_ = factor;
			(*iter)->renderState_.offsetUnit_ = unit;
		}
	}

	void Material::SetCullFace(int face)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.cullFace_ = face;
		}
	}

	void Material::SetPolygonMode(int mode)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.polygonMode_ = mode;
		}
	}

	void Material::SetPrimitive(int primitive)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.primitiveType_ = primitive;
		}
	}

	void Material::BlendFunc(int src, int dst)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->renderState_.srcBlendFunc_ = src;
			(*iter)->renderState_.dstBlendFunc_ = dst;
		}
	}

	void Material::AddPass(RenderPass* pass)
	{
		renderPasses_.push_back(pass);
	}

	void Material::RemovePass(int idx)
	{
		int i = 0;
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			if (i == idx)
			{
				delete* iter;
				renderPasses_.erase(iter);
				break;
			}
			++i;
		}
	}

	RenderPass* Material::GetPass(int idx)
	{
		int i = 0;
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			if (i == idx)
			{
				return *iter;
			}
			++i;
		}

		return nullptr;
	}

	bool Material::operator==(const Material& mat)
	{
		return false;
	}

	bool Material::operator==(const std::unordered_map<std::string, MaterialProperty*>& dataMap)
	{
		return false;
	}

	void Material::operator=(const Material& src)
	{
		for (auto iter = src.renderPasses_.begin(); iter != src.renderPasses_.end(); ++iter)
		{
			AddPass((*iter)->Clone());
		}

		bReceiveShadow_ = src.bReceiveShadow_;
		name_ = src.name_.text_;
		bSharedMaterial_ = false;
		if (!name_.EndWith("(Clone)"))
		{
			name_ << "(Clone)";
		}
	}

	UniformMatri4x4Property* Material::GetMatrix4Property(const char* uniformName)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			UniformMatri4x4Property* prop = (*iter)->GetMatrix4Property(uniformName);
			if (prop != nullptr)
			{
				return prop;
			}
		}

		return nullptr;
	}

	UniformVector4Property* Material::GetVec4Property(const char* uniformName)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			UniformVector4Property* prop = (*iter)->GetVec4Property(uniformName);
			if (prop != nullptr)
			{
				return prop;
			}
		}

		return nullptr;
	}

	void Material::SetTextureProperty(const char* uniformName, TextureUnit* texture)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->SetTextureProperty(uniformName, texture);
		}
	}

	void Material::SetTextureCubeProperty(const char* uniformName, TextureCube* texture)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->SetTextureCubeProperty(uniformName, texture);
		}
	}

	void Material::SetVec4Property(const char* uniformName, float x, float y, float z, float w)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->SetVec4Property(uniformName, x, y, z, w);
		}
	}

	void Material::SetVec4Property(const char* uniformName, float* val)
	{
		SetVec4Property(uniformName, val[0], val[1], val[2], val[3]);
	}

	void Material::SetVec4Property(const char* uniformName, const Serializer::Vector4f& val)
	{
		SetVec4Property(uniformName, val.x(), val.y(), val.z(), val.w());
	}

	void Material::SetVec4ArrayProperty(const char* uniformName, int size, float* val)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->SetVec4ArrayProperty(uniformName, size, val);
		}
	}

	void Material::SetMatrix4Property(const char* uniformName, const float* val)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->SetMatrix4Property(uniformName, val);
		}
	}

	void Material::SetMatrix3Property(const char* uniformName, float* val)
	{
		for (auto iter = renderPasses_.begin(); iter != renderPasses_.end(); ++iter)
		{
			(*iter)->SetMatrix3Property(uniformName, val);
		}
	}

	Material* Material::GetCachedMaterial(const char* path)
	{
		Material* pRet = nullptr;
		auto iter = s_cachedMaterials.find(path);
		if (iter != s_cachedMaterials.end())
		{
			pRet = iter->second;
		}

		return pRet;
	}

	void Material::UnloadMaterial(const char* name)
	{
	}

	void Material::CleanUpProperties()
	{

	}

	//static Material* CreateMaterialWithSerializedData(const char* name, Serializer::Material* mat)
	//{
	//	return nullptr;
	//}

	Material* Material::LoadMaterial(const char* name)
	{
		return nullptr;
	}
}