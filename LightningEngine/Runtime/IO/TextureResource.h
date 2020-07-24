#pragma once

#include "Resource.h"
#include "Serializer/TextureAtlas.serializer.h"

namespace LightningEngine
{
	enum TextureResourceType
	{
		kTextureResourceTypeTexture,
		kTextureResourceTypeTextureInfo,
		kTextureResourceTypeCount
	};

	class TextureResource : public Resource
	{
	public:
		static TextureResource* LoadTexture(Serializer::RuntimeAsset* pAsset);
		static TextureResource* CreateTextureResourceFromRawData(const char* path, Data& data);
		DECLEAR_ENGINE_CLASS(TextureResource)

	public:
		TextureResourceType textureResourceType_;
	};
}