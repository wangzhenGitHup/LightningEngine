#pragma once

#include "Resource.h"
#include"ResourceType.h"

namespace LightningEngine
{
	class ImageResource : public Resource
	{
	public:
		ImageResource();
		~ImageResource();
		static ImageResource* Init(const char* filepath);
		bool InitWithRawImageFileData(unsigned char* const data, int datalen);
		bool InitWithTextureAssetData(const unsigned char* data, int datalen);
		void Save(const char* filepath);

		DECLEAR_ENGINE_CLASS(ImageResource)

	public:
		Data pixelData_;
		int width_, height_;
		int bpp_;
	};
}