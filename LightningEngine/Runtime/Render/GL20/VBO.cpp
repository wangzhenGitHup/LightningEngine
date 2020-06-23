#include "VBO.h"

#include "Render/VertexData.h"
#include "Runtime/Debugger/Debugger.h"
#include "Render/RenderState.h"

namespace LightningEngine
{
	VBO::VBO(UInt16 nSize, GLenum hint) : 
		BufferObject(GL_ARRAY_BUFFER, nSize * sizeof(VertexDataFull), hint)
	{
		size_ = nSize;
		drawStartIdx_ = 0;
		drawCount_ = size_;
	}

	VBO::~VBO()
	{
		Active();
		GlobalRenderState::EnableAttribute(0, false);
		GlobalRenderState::EnableAttribute(1, false);
		GlobalRenderState::EnableAttribute(2, false);
		GlobalRenderState::EnableAttribute(3, false);
		GlobalRenderState::EnableAttribute(4, false);
		GlobalRenderState::EnableAttribute(5, false);
		GlobalRenderState::EnableAttribute(6, false);
		GlobalRenderState::EnableAttribute(7, false);
		Restore();
	}

	void VBO::Init(int count, GLenum hint)
	{
		size_ = count;
		drawCount_ = count;
		BufferObject::Init(GL_ARRAY_BUFFER, size_ * sizeof(VertexDataFull), hint);
	}

	void VBO::Draw(GLenum primitiveType)
	{
		glDrawArrays(primitiveType, drawStartIdx_, drawCount_);
	}
}