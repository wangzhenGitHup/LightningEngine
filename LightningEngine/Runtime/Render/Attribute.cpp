#include "Attribute.h"
#include "RenderState.h"
#include "Debugger/Debugger.h"

namespace LightningEngine
{
	Attribute::~Attribute()
	{
		if (nullptr != next_)
		{
			delete Next<Attribute>();
		}
	}

	void Attribute::Enable()
	{
		GlobalRenderState::EnableAttribute(attributeSlot_, true);
		OGL_CALL(glVertexAttribPointer(attributeSlot_, unitCount_, 
			dataType_, normalized_, 
			dataStride_, (void*)dataOffset_));
		if (nullptr != next_)
		{
			Next<Attribute>()->Enable();
		}
	}
}