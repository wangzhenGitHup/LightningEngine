#pragma once

#include "Base/Object.h"
#include "RuntimePrefix.h"
#include "String/FixedString.h"
#include "RenderMisc.h"
#include "Utils/SmartPtr.h"
#include "Math/Vector4.h"
#include "RenderState.h"
#include "RenderPass.h"
#include "RenderMisc.h"
#include "Serializer/Vector.serializer.h"
#include "Light/Light.h"
#include "MaterialProperty.h"
#include "GL20/Texture2D.h"
#include "GL20/GLSL.h"
#include "Serializer/Material.serializer.h"
#include "GL20/TextureCube.h"


namespace LightningEngine
{
	class TextureUnit;

	class Material : public Object
	{
	public:
		Material();
		virtual ~Material();
		void SetStencilBuffer();
		void RestoreStencilBuffer();
		void EnableBlend(bool bEnable);
		void BlendFunc(int src, int dst);
		void EnableZTest(bool bEnable);
		void EnableZWrite(bool bEnable);
		void EnableCull(bool bEnable);
		void EnablePointSprite(bool bEnable);
		void EnableWriteStencilMask(bool bEnable);
		void EnableUseStencilMask(bool bEnable);
		void ZTestFunction(int func);
		void ZOffset(float factor, float unit);
		void SetCullFace(int face);
		void SetPolygonMode(int mode);
		void SetPrimitive(int primitive);

		TextureUnitProperty* GetTextureProperty(const char* uniformName);
		UniformMatri4x4Property* GetMatrix4Property(const char* uniformName);
		UniformVector4Property* GetVec4Property(const char* uniformName);
		void SetTextureProperty(const char* uniformName, TextureUnit* texture);
		void SetTextureCubeProperty(const char* uniformName, TextureCube* texture);
		void SetVec4Property(const char* uniformName, float x, float y, float z, float w);
		void SetVec4Property(const char* uniformName, float* vec);
		void SetVec4Property(const char* uniformName, const Serializer::Vector4f& val);
		void SetVec4ArrayProperty(const char*uniformName, int size, float *v);
		void SetIntArrayProperty(const char*uniformName, int size, int *v);
		void SetMatrix4Property(const char*uniformName, const float*v);
		void SetMatrix3Property(const char*uniformName, float*v);
		void CleanUpProperties();
		void AddPass(RenderPass * pass);
		void RemovePass(int index);
		RenderPass * GetPass(int index);

		void operator=(const Material& mat);
		bool operator==(const Material& mat);
		bool operator==(const std::unordered_map<std::string, MaterialProperty*>& matMap);
		static Material* GetCachedMaterial(const char* matPath);
		static Material* LoadMaterial(const char*name);
		static void UnloadMaterial(const char*name);
		DECLEAR_ENGINE_CLASS(Material)

	public:
		static Material* s_default2DMaterial;
		static std::unordered_map<std::string, Material*> s_cachedMaterials;
		std::list<RenderPass*> renderPasses_;
		bool bCastShadow_;
		bool bReceiveShadow_;
		bool bSharedMaterial_;
		RenderingQueue renderingQueue_;
		static UniformVector4Property time_;
		static UniformVector4Property sinTime_;
		static UniformVector4Property cosTime_;
		
	};
}