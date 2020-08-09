#include "ImageResource.h"
#include "FileSystem.h"
#include "ResourceManager.h"
#include "String/StringUtils.h"
#include "Graphics/ImageUtils.h"
#include "Serializer/TextureAtlas.serializer.h"

namespace LightningEngine
{
	ImageResource::ImageResource() : width_(0), height_(0), bpp_(0)
	{
		type_ = ResourceTypeImage;
	}

	ImageResource::~ImageResource()
	{
	}

	ImageResource * ImageResource::Init(const char * filepath)
	{
		ImageResource* pImg = nullptr;
		Data data;
		if (ResourceManager::LoadData(filepath, data))
		{
			pImg = new ImageResource;
			Serializer::RuntimeAsset asset;
			if (asset.ParseFromArray(data.data_, data.dataLen_))
			{
				if (!pImg->InitWithTextureAssetData((const unsigned char*)asset.rawdata().c_str(),
					asset.rawdata().size()))
				{
					delete pImg;
					pImg = nullptr;
				}
			}
			else
			{
				if (!pImg->InitWithRawImageFileData(data.data_, data.dataLen_))
				{
					delete pImg;
					pImg = nullptr;
				}
			}
		}

		return pImg;
	}

	bool ImageResource::InitWithRawImageFileData(unsigned char * const data, int datalen)
	{
		bpp_ = RawImage2Uncompressed32BITRGBA(data, datalen, pixelData_, width_, height_);
		return bpp_ != 0;
	}

	bool ImageResource::InitWithTextureAssetData(const unsigned char * data, int datalen)
	{
		Serializer::TextureInfo info;
		if (info.ParseFromArray(data, datalen))
		{
			return InitWithRawImageFileData((unsigned char*)info.imagedata().c_str(), info.imagedata().size());
		}

		return false;
	}

	void ImageResource::Save(const char * filepath)
	{
		if (bpp_ == 32)
		{
			SaveRGBAToPNG32(filepath, pixelData_.data_, width_, height_);
		}
		else if (bpp_ == 24)
		{
			SaveRGBToJPEG24(filepath, pixelData_.data_, width_, height_);
		}
		else if (bpp_ == 8)
		{
			SaveRToJPEG8(filepath, pixelData_.data_, width_, height_);
		}
	}
}