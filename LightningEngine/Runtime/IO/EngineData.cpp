#include "EngineData.h"
#include "Allocator/TLSFAllocator.h"

namespace LightningEngine
{
	Data::Data(int buffer_size) : data_(nullptr), dataLen_(0), bufferSize_(buffer_size)
	{
	}

	Data::~Data()
	{
		if (0 != bufferSize_)
		{
			delete[] data_;
			dataLen_ = 0;
			bufferSize_ = 0;
			data_ = nullptr;
		}
	}

	void Data::Reset()
	{
		if (nullptr != data_)
		{
			delete[] data_;
			dataLen_ = 0;
			bufferSize_ = 0;
			data_ = nullptr;
		}
	}

	void Data::SetBufferSize(int buffer_size)
	{
		if (nullptr != data_)
		{
			Reset();
		}

		bufferSize_ = buffer_size;
		data_ = new(kMemDefaultId)UInt8[bufferSize_];
		dataLen_ = 0;
	}

	void Data::MakeZero()
	{
		memset(data_, 0, bufferSize_);
		dataLen_ = 0;
	}
}