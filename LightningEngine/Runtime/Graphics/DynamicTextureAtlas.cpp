#include "DynamicTextureAtlas.h"

namespace LightningEngine
{
	std::vector< DynamicTextureAtlas*> DynamicTextureAtlas::cachedAtlas_;

	DynamicTextureAtlas::DynamicTextureAtlas(UInt16 width, UInt16 height)
	{
		texture_ = new(kMemTextureId)Texture2D;
		texture_->InitTexture2D(width, height);
		texture_->name_ = "DynamicTextureAtlas";
		rootNode_ = new(kMemTextureId)TextureRectangleNode(true);
		rootNode_->rect_.Set(0, 0, width, height);
		Material* materail = new(kMemMaterialId)Material;
		material_ = materail;
		material_->SetTextureProperty("U_MainTexture", texture_);
		texture_->retain();
		cachedAtlas_.push_back(this);
	}

	TextureRectangleNode * DynamicTextureAtlas::AddTextureTile(UInt64 id, int width, int height, UInt8 * imageData)
	{
		if (textureTiles_.find(id) == textureTiles_.end())
		{
			TextureRectangleNode* newNode = rootNode_->AddTexture(width, height);
			if (nullptr != newNode)
			{
				textureTiles_.insert(std::pair<UInt64, TextureRectangleNode*>(id, newNode));
				texture_->UploadSubTexture(newNode->rect_.left_, newNode->rect_.bottom_, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
				newNode->texCoord_.x_ = (float)newNode->rect_.left_ / (float)rootNode_->rect_.width_; //left
				newNode->texCoord_.y_ = (float)newNode->rect_.bottom_ / (float)rootNode_->rect_.height_; //bottom
				newNode->texCoord_.z_ = (float)(newNode->rect_.left_ + newNode->rect_.width_) / (float)rootNode_->rect_.width_; //right
				newNode->texCoord_.w_ = (float)(newNode->rect_.bottom_ + newNode->rect_.height_) / (float)rootNode_->rect_.height_; //top
				return newNode;
			}

			return nullptr;
		}
		return textureTiles_[id];
	}

	DynamicTextureAtlas * DynamicTextureAtlas::GetDefaultDynamicTextureAtlas()
	{
		if (cachedAtlas_.empty())
		{
			return new DynamicTextureAtlas;
		}

		return cachedAtlas_.back();
	}
}