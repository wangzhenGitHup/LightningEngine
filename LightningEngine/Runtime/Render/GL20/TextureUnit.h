#pragma once

#include "RuntimePrefix.h"
#include "Base/Object.h"
#include "Render/LightningEngineGL.h"
#include "Utils/SmartPtr.h"

namespace LightningEngine
{
	class TextureUnit : public Object
	{
	public:
		TextureUnit();
		virtual ~TextureUnit();

	public:
		GLuint textureID_;
		GLenum textureType_;
	};
}