#include "TextureResource.h"
#include "ResourceManager.h"
#include "String/StringUtils.h"

namespace LightningEngine
{
	TextureResource * TextureResource::LoadTexture(Serializer::RuntimeAsset * pAsset)
	{
		Serializer::Texture* pTexture = new(kMemTextureId)Serializer::Texture;
		if (pTexture->ParseFromArray(pAsset->rawdata().c_str(), pAsset->rawdata().length()))
		{
			if (pTexture->has_image_data())
			{
				TextureResource* pRes = new(kMemTextureId)TextureResource;
				pRes->type_ = (ResourceType)pAsset->type();
				pRes->textureResourceType_ = kTextureResourceTypeTexture;
				return pRes;
			}
		}

		delete pTexture;

		Serializer::TextureInfo* pTextureInfo = new(kMemTextureId)Serializer::TextureInfo;
		if (pTextureInfo->ParseFromArray(pAsset->rawdata().c_str(), pAsset->rawdata().length()))
		{
			TextureResource* pRes = new(kMemTextureId)TextureResource;
			pRes->type_ = (ResourceType)pAsset->type();
			pRes->resourceData_ = pTextureInfo;
			pRes->textureResourceType_ = kTextureResourceTypeTextureInfo;
			return pRes;
		}

		delete pTextureInfo;
		return nullptr;
	}

	TextureResource * TextureResource::CreateTextureResourceFromRawData(const char * path, Data & data)
	{
		Serializer::TextureInfo* pTextureInfo = new(kMemTextureId)Serializer::TextureInfo;
		pTextureInfo->set_imagedata(data.data_, data.dataLen_);

		TextureResource* pRes = new(kMemTextureId)TextureResource;
		if (StringUtils::EndWithI(path, ".png"))
		{
			pRes->type_ = ResourceTypePNGImage;
		}
		else if (StringUtils::EndWithI(path, ".jpg"))
		{
			pRes->type_ = ResourceTypeJPGImage;
		}
		else if (StringUtils::EndWithI(path, ".bmp"))
		{
			pRes->type_ = ResourceTypeBMPImage;
		}

		pRes->resourceData_ = pTextureInfo;
		pRes->textureResourceType_ = kTextureResourceTypeTextureInfo;
		return pRes;
	}
}