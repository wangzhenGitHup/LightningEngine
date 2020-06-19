#pragma once

#include "Runtime/Utils/LinkedList.h"
#include "Runtime/RuntimePrefix.h"
#include "Runtime/Render/LightningEngineGL.h"

namespace LightningEngine
{
	class Attribute : public LinkedList
	{
	public:
		Attribute() : attributeSlot_(-1), dataOffset_(0), normalized_(GL_FALSE) {}

		~Attribute();
		void Enable();

	public:
		Int32 attributeSlot_;
		Int32 dataOffset_;
		GLenum dataType_;
		GLboolean normalized_;
		UInt8 unitCount_;
		UInt16 dataStride_;
	};
}