#pragma once

#include "RuntimePrefix.h"
#include "Render/LightningEngineGL.h"

namespace LightningEngine
{
	class BufferObject
	{
	public:
		BufferObject(GLenum type, int size, GLenum hint);
		virtual ~BufferObject();
		virtual void Init(GLenum type, int size, GLenum hint);
		void SubData(const void* pData, int size, int offset = 0);
		bool Active();
		bool Restore();

	public:
		GLuint identifier_;
		GLenum type_;
		int size_;
	};
}