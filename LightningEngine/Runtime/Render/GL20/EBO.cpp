#include "EBO.h"
#include "Debugger/Debugger.h"

namespace LightningEngine
{
	EBO::EBO(int iCount, GLenum hint) : BufferObject(GL_ELEMENT_ARRAY_BUFFER, iCount * sizeof(UInt16), hint)
	{
		indexCount_ = iCount;
		indexCountForDraw_ = iCount;
		startIndex_ = 0;
	}

	EBO::~EBO()
	{
	}

	void EBO::Init(int idxCnt, GLenum hint)
	{
		indexCount_ = idxCnt;
		indexCountForDraw_ = idxCnt;
		BufferObject::Init(GL_ELEMENT_ARRAY_BUFFER, indexCount_ * sizeof(UInt16), hint);
	}

	void EBO::Draw(GLenum primitiveType)
	{
		if (Active())
		{
			glDrawElements(primitiveType, indexCountForDraw_, 
				GL_UNSIGNED_SHORT, (void*)(sizeof(UInt16) * startIndex_));
			Restore();
		}
	}
}