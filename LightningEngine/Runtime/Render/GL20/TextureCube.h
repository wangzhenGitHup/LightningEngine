#pragma once

#include "RuntimePrefix.h"
#include "Base/Object.h"
#include "Render/LightningEngineGL.h"
#include "Utils/SmartPtr.h"
#include "IO/Resource.h"
#include "IO/TextureResource.h"
#include "TextureUnit.h"

namespace LightningEngine
{
	class TextureCube : public TextureUnit
	{
	public:
		TextureCube();
		~TextureCube();
		static TextureCube* LoadTextureCube(const char* pName);
		void InitTextureCube();

		DECLEAR_ENGINE_CLASS(TextureCube)

	public:
		GLenum minFilter_;
		GLenum magFilter_;
		GLenum filterDst_;
		GLenum filterSrc_;
		GLenum internalFormat_;
		GLenum srcFormat_;
		GLuint width_, height_;
		SmartPtr<Resource> leftRes_, rightRes_, frontRes_, backRes_, topRes_, bottomRes_;
		static std::unordered_map<std::string, TextureCube*> textureCubes_;
	};
}