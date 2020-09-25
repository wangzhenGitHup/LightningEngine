#pragma once

#include "Base/Object.h"
#include "Render/LightningEngineGL.h"
#include "Render/GL20/Texture2D.h"
#include "Render/Material.h"
#include "TextureRectangleNode.h"
#include "Utils/SmartPtr.h"


namespace LightningEngine
{
	class DynamicTextureAtlas : public Object
	{
	public:
		DynamicTextureAtlas(UInt16 width = 4096, UInt16 height = 4096);
		TextureRectangleNode* AddTextureTile(UInt64 id, int width, int height, UInt8* imageData);
		static DynamicTextureAtlas* GetDefaultDynamicTextureAtlas();

	public:
		std::map<UInt64, TextureRectangleNode*> textureTiles_;
		TextureRectangleNode* rootNode_;
		Texture2D* texture_;
		SmartPtr<Material> material_;
		static std::vector< DynamicTextureAtlas*> cachedAtlas_;
	};
}