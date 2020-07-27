#pragma once

#include "RuntimePrefix.h"
#include "Render/LightningEngineGL.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"
#include "Base/Component.h"

namespace LightningEngine
{
	enum LightSourceType
	{
		kLightSourceTypeDirection = 1,
		kLightSourceTypePoint = 2,
		kLightSourceTypeSpot = 3
	};

	class Light : public Component
	{
	public:
		Light();
		virtual ~Light();
		void Update(float deltaTime);

		DECLEAR_ENGINE_CLASS(Light)

	public:
		Vector4 position_;
		Color4 ambientColor_, diffuseColor_, specularColor_;
		Vector4 vecSetting_, vecSetting1_;
		Matrix4x4 projection_, view_;
		UInt32 cullingMask_;
		LightSourceType type_;
		static std::set<Light*> s_lightSet;
		static int s_lightEnableMask[8];
		static Vector4 s_lightPos[8];
		static Vector4 s_lightAmbient[8];
		static Vector4 s_lightDiffuse[8];
		static Vector4 s_lightSpecular[8];
		static Vector4 s_lightSetting[8];
		static Vector4 s_lightSetting1[8];
		static Light* s_mainLight;
	};
}