#pragma once

#include "BufferObject.h"

namespace LightningEngine
{
	class EBO : public BufferObject
	{
	public:
		EBO(int iCount = 0, GLenum hint = GL_DYNAMIC_DRAW);
		virtual ~EBO();
		void Init(int idxCnt, GLenum hint = GL_DYNAMIC_DRAW);
		void Draw(GLenum primitiveType);

	public:
		UInt16 indexCount_;
		UInt16 indexCountForDraw_;
		UInt16 startIndex_;
	};
}