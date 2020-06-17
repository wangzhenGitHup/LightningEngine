#include "TextureUnit.h"
#include "Debugger/Debugger.h"

namespace LightningEngine
{
	TextureUnit::TextureUnit() : Object(1), textureID_(0), textureType_(GL_NONE)
	{
	}

	TextureUnit::~TextureUnit()
	{
		if (textureID_ != 0)
		{
			OGL_CALL(glDeleteTextures(1, &textureID_));
		}
	}
}