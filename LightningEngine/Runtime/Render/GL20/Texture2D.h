#pragma once

#include "RuntimePrefix.h"
#include "Render/LightningEngineGL.h"
#include "Render/RenderMisc.h"
#include "IO/TextureResource.h"
#include "Serializer/TextureInfo.serializer.h"
#include "Utils/SmartPtr.h"
#include "TextureUnit.h"
#include <map>
#include <string>

namespace LightningEngine
{
	class Texture2D : public TextureUnit
	{
	public:
		Texture2D();
		~Texture2D();
		void InitTexture2D(UInt16 width = 1024,
			UInt16 height = 1024,
			GLenum internalformat = GL_RGBA,
			GLenum srcFormat = GL_RGBA,
			GLenum minFilter = GL_LINEAR,
			GLenum magFilter = GL_LINEAR,
			GLenum srcFilter = GL_CLAMP_TO_EDGE,
			GLenum dstFilter = GL_CLAMP_TO_EDGE);

		void InitDepthTexture(UInt16 width = 1024, UInt16 height = 1024);
		void UpLoadTexture(Any imageData);
		void TexParameteri(GLenum name, GLenum value);
		void UploadETC1RGBTexture(UInt8* imageData, UInt16 paddedWidth, UInt16 paddedHeight, UInt8* alphaChannel = nullptr);
		void UploadSubTexture(UInt16 x, UInt16 y, UInt16 width, UInt16 height, GLenum format, GLenum dataType, UInt8* pData);
		Serializer::Sprite* GetSprite(const char* name);
		void InitSpriteInfo(Serializer::Texture* texture);
		static Texture2D* CreateTextureFromMemory(const char* pData, int dataLen, 
			GLenum minFilter = GL_LINEAR,
			GLenum magFilter = GL_LINEAR,
			GLenum srcFilter = GL_CLAMP_TO_EDGE,
			GLenum dstFilter = GL_CLAMP_TO_EDGE);
		
		static Texture2D* CreateTextureWidthFreeImage(TextureResource* pJPG);
		static Texture2D* CreateTextureFromETC1RGBFile(TextureResource* pETC1RGB);
		static Texture2D* CreateTextureFromPVRTC4RGBAFile(TextureResource* pPVRTCRGBA);
		static Texture2D* CreateTextureFromRawLuminance8(TextureResource* pRawData);
		static Texture2D* GetCachedTexture2D(const char* pPath);
		static Serializer::ImageTile* GetSpriteInfo(const char* pAtlasName, const char* pSpriteName);
		static void UnLoadTexture(const char* pName);
		static Texture2D* LoadTexture2D(const char* pName);

		DECLEAR_ENGINE_CLASS(Texture2D)

	public:
		GLenum minFilter_;
		GLenum magFilter_;
		GLenum srcFilter_;
		GLenum dstFilter_;
		GLenum internalFormat_;
		GLenum srcFormat_;
		GLuint width_, height_;
		SmartPtr<Resource> res_;
		std::unordered_map<std::string, Serializer::Sprite*> sprites_;
		static std::unordered_map<std::string, Texture2D*> textures_;
		static Texture2D* LoadTexture2D(const char* name);
		static  int textureMemoryCount_;
		static Texture2D* textureError_;

	};
}