#include "Texture2D.h"
#include "IO/ResourceManager.h"
#include "Graphics/ImageUtils.h"
#include "Plugins/FreeImage/FreeImage.h"
#include "String/StringUtils.h"
#include "Serializer/TextureAtlas.serializer.h"
#include "Render/RenderState.h"
#include "Debugger/Debugger.h"


namespace LightningEngine
{
	Texture2D* Texture2D::textureError_;
	std::unordered_map<std::string, Texture2D*> Texture2D::textures_;
	static std::map<std::string, Serializer::TextureAtlas*> s_cachedAtlas;
	int Texture2D::textureMemoryCount_ = 0;


	Texture2D::Texture2D() : res_(nullptr),
		width_(0),
		height_(0)
	{
		textureType_ = GL_TEXTURE_2D;
		textureID_ = 0;
	}

	Texture2D::~Texture2D()
	{
		std::unordered_map<std::string, Texture2D*>::iterator iter = textures_.find(name_.text_);
		if (iter != textures_.end())
		{
			textures_.erase(iter);
		}
	}

	void Texture2D::InitTexture2D(UInt16 width, UInt16 height, 
		GLenum internalformat, GLenum srcFormat, 
		GLenum minFilter, GLenum magFilter, 
		GLenum srcFilter, GLenum dstFilter)
	{
		GLenum internalFormat;
		GLenum srcFormat;
		GLenum minFilter;
		GLenum magFilter;
		GLenum srcFilter;
		GLenum dstFilter;
		if (0 != textureID_)
		{
			OGL_CALL(glDeleteTextures(1, &textureID_));
			textureID_ = 0;
		}

		width_ = width;
		height_ = height;
		internalFormat_ = internalFormat;
		srcFormat_ = srcFormat;
		minFilter_ = minFilter;
		magFilter_ = magFilter;
		dstFilter_ = dstFilter;
		srcFilter_ = srcFilter;
		OGL_CALL(glGenTextures(1, &textureID_));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, textureID_);
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter_));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter_));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, srcFilter_));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dstFilter_));
		OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat_, 
			width_, height_, 
			0, srcFormat_, 
			GL_UNSIGNED_BYTE, nullptr));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::InitDepthTexture(UInt16 width, UInt16 height)
	{
		width_ = width;
		height_ = height;
		internalFormat_ = GL_DEPTH_COMPONENT;
		srcFormat_ = GL_DEPTH_COMPONENT;
		minFilter_ = GL_LINEAR;
		magFilter_ = GL_LINEAR;
		dstFilter_ = GL_CLAMP_TO_EDGE;
		srcFilter_ = GL_CLAMP_TO_EDGE;
		OGL_CALL(glGenTextures(1, &textureID_));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, textureID_);
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter_));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter_));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, srcFilter_));
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dstFilter_));
		OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, 
			internalFormat_, width_, 
			height_, 0, 
			srcFormat_, GL_FLOAT, nullptr));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::UpLoadTexture(Any imageData)
	{
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, textureID_);
		OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat_, 
			(GLsizei)width_, (GLsizei)height_, 
			0, srcFormat_, 
			GL_UNSIGNED_BYTE, imageData));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::TexParameteri(GLenum name, GLenum value)
	{
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, textureID_);
		OGL_CALL(glTexParameteri(GL_TEXTURE_2D, name, value));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::UploadETC1RGBTexture(UInt8 * imageData, UInt16 paddedWidth, 
		UInt16 paddedHeight, UInt8 * alphaChannel)
	{
#if LIGHTNING_ENGINE_PLATFORM_ANDROID
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, textureID_);
		OGL_CALL(glCompressedTexImage2D(GL_TEXTURE_2D, 0, 
			internalFormat_, (GLsizei)width_, 
			(GLsizei)height_, 0, 
			(paddedWidth * paddedHeight) >> 1, 
			imageData + 16));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
#endif
	}

	void Texture2D::UploadSubTexture(UInt16 x, UInt16 y, 
		UInt16 width, UInt16 height, 
		GLenum format, GLenum dataType, 
		UInt8 * pData)
	{
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, textureID_);
		OGL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0,
			x, y,
			width, height,
			format, dataType, pData));
		GlobalRenderState::BindTexture(GL_TEXTURE_2D, 0);
	}

	Serializer::Sprite * Texture2D::GetSprite(const char * name)
	{
		auto iter = sprites_.find(name);
		if (iter != sprites_.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	void Texture2D::InitSpriteInfo(Serializer::Texture * texture)
	{
		int count = texture->sprites_size();
		for (int i = 0; i < count; i++)
		{
			Serializer::Sprite* pSprite = new Serializer::Sprite;
			*pSprite = texture->sprites(i);
			sprites_.insert(std::pair<std::string, Serializer::Sprite*>(pSprite->name(), pSprite));
		}
	}

	Texture2D * Texture2D::CreateTextureFromMemory(const char * pData, int dataLen, 
		GLenum minFilter, GLenum magFilter, 
		GLenum srcFilter, GLenum dstFilter)
	{
		Data data;
		GLenum cpuFormat = GL_RGBA;
		GLenum gpuFormat = GL_RGBA;
		int width = 0;
		int height = 0;
		int bpp = RawImage2Uncompressed32BITRGBA((UInt8*)pData, dataLen, data, width, height);
		switch (bpp)
		{
		case 32:
			break;

		case 24:
			cpuFormat = GL_RGB;
			gpuFormat = GL_RGB;
			break;

		case 8:
			cpuFormat = GL_ALPHA;
			gpuFormat = GL_ALPHA;
			break;

		default:
			ErrorPrint("file bpp is not supported, must be 8 or 24 or 32");
			return nullptr;
		}

		Texture2D* texture2D = new Texture2D;
		texture2D->InitTexture2D(width, height, 
			gpuFormat, cpuFormat, 
			minFilter, magFilter, 
			srcFilter, dstFilter);
		texture2D->UpLoadTexture(data.data_);

		return texture2D;
	}

	Texture2D * Texture2D::CreateTextureWidthFreeImage(TextureResource * pJPG)
	{
		Texture2D* pRet = nullptr;
		switch (pJPG->textureResourceType_)
		{
		case kTextureResourceTypeTexture:
		{
			Serializer::Texture* pTex = pJPG->RuntimeData<Serializer::Texture>();
			pRet = CreateTextureFromMemory(
				pTex->image_data().c_str(), pTex->image_data().length(),
				pTex->min_filter(), pTex->mag_filter(), 
				pTex->wrap_mode_u(), pTex->wrap_mode_v());
			pRet->InitSpriteInfo(pTex);
		}
		break;

		case kTextureResourceTypeTextureInfo:
		{
			Serializer::TextureInfo* pInfo = pJPG->RuntimeData<Serializer::TextureInfo>();
			pRet = CreateTextureFromMemory(pInfo->imagedata().c_str(),
				pInfo->imagedata().length());
		}
		break;
		}
		return pRet;
	}

	Texture2D * Texture2D::CreateTextureFromETC1RGBFile(TextureResource * pETC1RGB)
	{
#if LIGHTNING_ENGINE_PLATFORM_ANDROID
		Serializer::ETC1Data etc1Data;
		Serializer::TextureInfo* pInfo = pETC1RGB->RuntimeData<Serializer::TextureInfo>();
		if (etc1Data.ParseFromArray(pInfo->imagedata().c_str(), pInfo->imagedata().length()))
		{
			UInt8* imageData = (UInt8*)etc1Data.mutable_rgb()->c_str();
			UInt16 width = (imageData[12] << 8) | imageData[13];
			UInt16 height = (imageData[14] << 8) | imageData[15];
			UInt16 paddedWidth = (imageData[8] << 8) | imageData[9];
			UInt16 paddedHeight = (imageData[10] << 8) | imageData[11];

			Texture2D* pTex = new Texture2D;
			pTex->InitTexture2D(width, height, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_ETC1_RGB8_OES, GL_RGB);
			UInt8* alphaData = nullptr;
			if (etc1Data.has_alpha())
			{

			}

			pTex->UploadETC1RGBTexture(imageData, paddedWidth, paddedHeight, alphaData);
			return pTex;
		}
#endif
		return textureError_;
	}

	Texture2D * Texture2D::CreateTextureFromPVRTC4RGBAFile(TextureResource * pPVRTCRGBA)
	{
		return textureError_;
	}

	Texture2D * Texture2D::CreateTextureFromRawLuminance8(TextureResource * pRawData)
	{
		Serializer::TextureInfo* pInfo = pRawData->RuntimeData<Serializer::TextureInfo>();
		Texture2D* pTex = new Texture2D;

		return pTex;
	}

	Texture2D * Texture2D::GetCachedTexture2D(const char * pPath)
	{
		if (nullptr == pPath)
		{
			return nullptr;
		}

		auto iter = textures_.find(pPath);
		if (iter != textures_.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	Serializer::ImageTile * Texture2D::GetSpriteInfo(const char * pAtlasName, const char * pSpriteName)
	{
		std::map<std::string, Serializer::TextureAtlas*>::iterator iter = s_cachedAtlas.find(pAtlasName);
		if (iter != s_cachedAtlas.end())
		{
			int iCnt = iter->second->imagetiles_size();
			for (int i = 0; i < iCnt; i++)
			{
				if (strcmp(pSpriteName, iter->second->imagetiles(i).name().c_str()) == 0)
				{
					return iter->second->mutable_imagetiles(i);
				}
			}
		}
		return nullptr;
	}

	void Texture2D::UnLoadTexture(const char * pName)
	{
		if (nullptr == pName)
		{
			return;
		}

		Texture2D* pTex = nullptr;
		std::unordered_map<std::string, Texture2D*>::iterator iter = textures_.find(pName);
		if (iter != textures_.end())
		{
			pTex = iter->second;
			textures_.erase(iter);
			Resource::Unload(pName);
		}
	}

	Texture2D* Texture2D::LoadTexture2D(const char* pName)
	{
		if (nullptr == pName || strlen(pName) == 0)
		{
			return textureError_;
		}

		std::unordered_map<std::string, Texture2D*>::iterator iter = textures_.find(pName);
		if (iter != textures_.end())
		{
			return iter->second;
		}

		TextureResource* pTexRes = Resource::LoadResource<TextureResource>(pName);
		if (nullptr == pTexRes)
		{
			ErrorPrint("Texture2D::LoadTexture2D load %s failed", pName);
			return textureError_;
		}

		Texture2D* pTex = nullptr;
		switch (pTexRes->type_)
		{
		case ResourceTypePNGImage:
		case ResourceTypeBMPImage:
		case ResourceTypeTGAImage:
		case ResourceTypeJPGImage:
			pTex = CreateTextureWidthFreeImage(pTexRes);
			break;

		case ResourceTypeETC1RGB:
			pTex = CreateTextureFromETC1RGBFile(pTexRes);
			break;

		case ResourceTypePVRTCRGBA:
			pTex = CreateTextureFromPVRTC4RGBAFile(pTexRes);
			break;

		case ResourceTypeRawImageLUMINANCE:
			pTex = CreateTextureFromRawLuminance8(pTexRes);
			break;
		}

		delete pTexRes;
		if (nullptr == pTex)
		{
			ErrorPrint("Texture2D::LoadTexture2D decode %s failed", pName);
			return textureError_;
		}

		pTex->name_ = pName;
		textures_.insert(std::pair<std::string, Texture2D*>(pName, pTex));
		return pTex;
	}
}
