#include "Light.h"
#include "Scene/GameObject.h"

namespace LightningEngine
{
	Light* Light::s_mainLight = nullptr;
	std::set<Light*> Light::s_lightSet;
	int Light::s_lightEnableMask[8];
	Vector4 Light::s_lightPos[8];
	Vector4 Light::s_lightAmbient[8];
	Vector4 Light::s_lightDiffuse[8];
	Vector4 Light::s_lightSpecular[8];
	Vector4 Light::s_lightSetting[8];
	Vector4 Light::s_lightSetting1[8];

	Light::Light()
	{
		type_ = kLightSourceTypeDirection;
		position_.Set(0.0f, 1.0f, 0.0f, 0.0f);
		ambientColor_.Set(1.0f, 1.0f, 1.0f, 1.0f);
		diffuseColor_.Set(1.0f, 1.0f, 1.0f, 1.0f);
		specularColor_.Set(1.0f, 1.0f, 1.0f, 1.0f);
	}

	Light::~Light()
	{
		auto iter = s_lightSet.find(this);
		if (iter != s_lightSet.end())
		{
			s_lightSet.erase(iter);
		}
	}

	void Light::Update(float deltaTime)
	{
		if (bPositionChanged_)
		{
			bPositionChanged_ = false;
			switch (type_)
			{
			case kLightSourceTypeDirection:
				position_.x_ = owner_->localTransform.position_.x_;
				position_.x_ = owner_->localTransform.position_.y_;
				position_.x_ = owner_->localTransform.position_.z_;
				position_.w_ = 0.0f;
				break;

			case kLightSourceTypePoint:
				const Matrix4x4& world_matrix = owner_->GetWorldMatrix();
				position_.x_ = world_matrix.data_[12];
				position_.y_ = world_matrix.data_[13];
				position_.z_ = world_matrix.data_[14];
				position_.w_ = 1.0f;
				break;

			case kLightSourceTypeSpot:
				const Matrix4x4& world_matrix = owner_->GetWorldMatrix();
				position_.x_ = world_matrix.data_[12];
				position_.y_ = world_matrix.data_[13];
				position_.z_ = world_matrix.data_[14];
				break;
			}
		}
	}

}