#include "BinaryBuffer.h"

namespace LightningEngine
{
	BinaryBuffer::BinaryBuffer() : buffer_(nullptr),
		dataSize_(0),
		size_(0)
	{
	}

	BinaryBuffer::~BinaryBuffer()
	{
		if (0 != size_)
		{
			delete[] buffer_;
			buffer_ = nullptr;
		}
	}

	void BinaryBuffer::SetSize(int size)
	{
		size_ = size;
		buffer_ = new UInt8[size];
		memset(buffer_, 0, size);
	}

	void BinaryBuffer::AppendInt(int val)
	{
		memcpy(buffer_ + dataSize_, &val, sizeof(int));
		dataSize_ += sizeof(int);
	}

	void BinaryBuffer::Append(const char * data, int cnt)
	{
		memcpy(buffer_ + dataSize_, data, cnt);
		dataSize_ += cnt;
	}

	void BinaryBuffer::Shift(int cnt)
	{
		if (0 == cnt)
		{
			return;
		}

		if (cnt >= dataSize_)
		{
			Reset();
		}
		else
		{
			int iDataLeft = dataSize_ - cnt;
			memcpy(buffer_, buffer_ + cnt, iDataLeft);
			memset(buffer_ + iDataLeft, 0, cnt);
			dataSize_ -= cnt;
		}
	}

	void BinaryBuffer::Reset()
	{
		if (size_ > 0)
		{
			memset(buffer_, 0, size_);
			dataSize_ = 0;
		}
	}

	Int32 BinaryBuffer::GetNextInt(const char * buffer)
	{
		Int32 val = *((Int32*)buffer);
		if (val < 0)
		{
			return -1;
		}

		return val;
	}
}