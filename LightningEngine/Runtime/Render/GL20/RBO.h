#pragma once

#include "RuntimePrefix.h"
#include "Render/LightningEngineGL.h"

namespace LightningEngine
{
	class RBO
	{
	public:
		RBO();
		~RBO();
		void Init(int width, int height);
		void Resize(int width, int height);

	public:
		GLuint renderBufferObject_;
	};
}