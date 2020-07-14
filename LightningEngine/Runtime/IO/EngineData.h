#pragma once

#include "RuntimePrefix.h"

namespace LightningEngine
{
	class Data
	{
	public:
		Data(int buffer_size = 0);
		virtual ~Data();
		void Reset();
		void SetBufferSize(int buffer_size);
		void MakeZero();

	public:
		UInt8* data_;
		Int32 dataLen_;
		Int32 bufferSize_;
	};
}