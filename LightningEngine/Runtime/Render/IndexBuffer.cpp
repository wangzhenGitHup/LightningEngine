#include "IndexBuffer.h"

namespace LightningEngine
{
	IndexBuffer::IndexBuffer(UInt16 size) : indexes_(nullptr)
	{
		size_ = size;
		bufferSize_ = 0;
		if (0 != size_)
		{
			indexes_ = new UInt16[size_];
			bufferSize_ = sizeof(UInt16) * size_;
			memset(indexes_, 0, bufferSize_);
		}
	}

	IndexBuffer::~IndexBuffer()
	{
		if (nullptr != indexes_)
		{
			delete indexes_;
			indexes_ = nullptr;
		}

		size_ = 0;
		bufferSize_ = 0;
	}

	void IndexBuffer::Init(UInt16 size)
	{
		size_ = size;
		indexes_ = new UInt16[size_];
		bufferSize_ = sizeof(UInt16) * size_;
		memset(indexes_, 0, bufferSize_);
	}
}