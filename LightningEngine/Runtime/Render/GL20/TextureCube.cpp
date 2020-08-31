#include "TextureCube.h"
#include "Debugger/Debugger.h"
#include "IO/ResourceManager.h"
#include "Serializer/TextureInfo.serializer.h"
#include "String/StringUtils.h"
#include "IO/FileSystem.h"
#include "Graphics/ImageUtils.h"
#include "Render/RenderState.h"

namespace LightningEngine
{
	std::unordered_map<std::string, TextureCube*> TextureCube::textureCubes_;

	TextureCube::TextureCube()
	{
		OGL_CALL(glGenTextures(1, &textureID_));
		textureType_ = GL_TEXTURE_CUBE_MAP;
	}

	TextureCube::~TextureCube()
	{
		std::unordered_map<std::string, TextureCube*>::iterator iter = textureCubes_.find(name_.text_);
		if (iter != textureCubes_.end())
		{
			textureCubes_.erase(iter);
		}
	}

	TextureCube * TextureCube::LoadTextureCube(const char * pName)
	{
		if (nullptr == pName || strlen(pName) == 0)
		{
			return nullptr;
		}
		
		std::unordered_map<std::string, TextureCube*>::iterator iter = textureCubes_.find(pName);
		if (iter != textureCubes_.end())
		{
			return iter->second;
		}

		char tmp[256];
		memset(tmp, 0, 256);
		strcpy(tmp, pName);
		strcat(tmp, "/front.jpg");
		TextureResource* pFrontRes = Resource::LoadResource<TextureResource>(tmp);

		memset(tmp, 0, 256);
		strcpy(tmp, pName);
		strcat(tmp, "/back.jpg");
		TextureResource* pBackRes = Resource::LoadResource<TextureResource>(tmp);

		memset(tmp, 0, 256);
		strcpy(tmp, pName);
		strcat(tmp, "/left.jpg");
		TextureResource* pLeftRes = Resource::LoadResource<TextureResource>(tmp);

		memset(tmp, 0, 256);
		strcpy(tmp, pName);
		strcat(tmp, "/right.jpg");
		TextureResource* pRightRes = Resource::LoadResource<TextureResource>(tmp);

		memset(tmp, 0, 256);
		strcpy(tmp, pName);
		strcat(tmp, "/top.jpg");
		TextureResource* pTopRes = Resource::LoadResource<TextureResource>(tmp);

		memset(tmp, 0, 256);
		strcpy(tmp, pName);
		strcat(tmp, "/bottom.jpg");
		TextureResource* pBottomRes = Resource::LoadResource<TextureResource>(tmp);

		TextureCube* pTexCube = new TextureCube;
		pTexCube->frontRes_ = pBackRes;
		pTexCube->backRes_ = pFrontRes;
		pTexCube->leftRes_ = pLeftRes;
		pTexCube->rightRes_ = pRightRes;
		pTexCube->topRes_ = pBottomRes;
		pTexCube->bottomRes_ = pTopRes;
		pTexCube->name_ = pName;
		pTexCube->InitTextureCube();
		textureCubes_.insert(std::pair<std::string, TextureCube*>(pName, pTexCube));
		return pTexCube;
	}

	void TextureCube::InitTextureCube()
	{
		GlobalRenderState::BindTexture(GL_TEXTURE_CUBE_MAP, textureID_);
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		OGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		Data pixelData;
		int width = 0, height = 0;
		Serializer::TextureInfo* pTexInfo = frontRes_->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)pTexInfo->imagedata().c_str(), 
			pTexInfo->imagedata().size(), pixelData, width, height);

		//front
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 
			width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.data_));

		//back
		pixelData.Reset();
		width = 0, height = 0;
		pTexInfo = backRes_->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)pTexInfo->imagedata().c_str(), 
			pTexInfo->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB,
			width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.data_));

		//left
		pixelData.Reset();
		width = 0, height = 0;
		pTexInfo = leftRes_->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)pTexInfo->imagedata().c_str(), 
			pTexInfo->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, pixelData.data_));

		//right
		pixelData.Reset();
		width = 0, height = 0;
		pTexInfo = rightRes_->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)pTexInfo->imagedata().c_str(), 
			pTexInfo->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 
			0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.data_));

		//top
		pixelData.Reset();
		width = 0, height = 0;
		pTexInfo = topRes_->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)pTexInfo->imagedata().c_str(),
			pTexInfo->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, 
			GL_RGB, GL_UNSIGNED_BYTE, pixelData.data_));

		//bottom
		pixelData.Reset();
		width = 0, height = 0;
		pTexInfo = bottomRes_->RuntimeData<Serializer::TextureInfo>();
		RawImage2Uncompressed32BITRGBA((unsigned char*)pTexInfo->imagedata().c_str(), 
			pTexInfo->imagedata().size(), pixelData, width, height);
		OGL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, pixelData.data_));
		GlobalRenderState::BindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}