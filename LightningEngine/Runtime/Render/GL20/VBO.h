#pragma once

#include "BufferObject.h"

namespace LightningEngine
{
	class VBO : public BufferObject
	{
	public:
		VBO(UInt16 nSize = 0, GLenum hint = GL_DYNAMIC_DRAW);
		virtual ~VBO();
		void Init(int count, GLenum hint = GL_DYNAMIC_DRAW);
		void Draw(GLenum primitiveType);

	public:
		UInt16 size_;
		UInt16 drawStartIdx_;
		UInt16 drawCount_;
	};
}