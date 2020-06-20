#pragma once

#include "RuntimePrefix.h"

namespace LightningEngine
{
	class IndexBuffer
	{
	public:
		IndexBuffer(UInt16 size = 0);
		~IndexBuffer();
		void Init(UInt16 size);

	public:
		Int32 size_;
		Int32 bufferSize_;
		UInt16* indexes_;
	};
}