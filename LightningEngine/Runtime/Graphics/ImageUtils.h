#pragma once

#include  "RuntimePrefix.h"
#include "Plugins/FreeImage/FreeImage.h"
#include "IO/EngineData.h"

namespace LightningEngine
{
	int RawImage2Uncompressed32BITRGBA(unsigned char* const pData, int dataLen, 
		Data& outPixel, int& width, int& height);
	bool GetImageInfo(unsigned char* const pData, int dataLen, int& width, int& height, int& bpp);
	bool SaveFreeTypeFontToPNG32(const char* pFilePath, UInt8* pImgData, Int32 width, Int32 height);
	bool SaveRGBAToPNG32(const char* pFilePath, UInt8* pImgData, Int32 width, Int32 height);
	bool SaveRGBToJPEG24(const char* pFilePath, UInt8* pImgData, Int32 width, Int32 height);
	bool SaveRToJPEG8(const char* pFilePath, UInt8* pImgData, Int32 width, Int32 height);
	void BGRAToRGBA(unsigned char* pixel, int width, int height);
	void BGRToRGB(unsigned char* pixel, int width, int height);
	void AlphaToRGBA(unsigned char luminance, unsigned char* pixel, int width, int height, unsigned char* out);
	Any GenAlphaGradiantQuadImage(int size, float e = 8.0f);
	Any GenRGBImage(int width, int height, UInt8 r, UInt8 g, UInt8 b);

	class ImageUtils
	{
	public:
		static void Init();
		static void OnImageOperationException(FREE_IMAGE_FORMAT fif, const char* message);

	};
}