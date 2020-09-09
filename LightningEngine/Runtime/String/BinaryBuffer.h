#pragma once

#include "Base/Object.h"

namespace LightningEngine
{
	class BinaryBuffer : public Object
	{
	public:
		BinaryBuffer();
		~BinaryBuffer();
		void SetSize(int size);
		void AppendInt(int val);
		void Append(const char* data, int cnt);
		void Shift(int cnt);
		void Reset();
		static Int32 GetNextInt(const char* buffer);
		int GetDataSize() { return dataSize_; }
		void SetDataSize(int size) { dataSize_ = size; }
		UInt8* GetBuffer() { return buffer_; }

		DECLEAR_ENGINE_CLASS(BinaryBuffer)

	private:
		BinaryBuffer(const BinaryBuffer& val);
		BinaryBuffer& operator=(const BinaryBuffer& val);
		

	private:
		UInt8* buffer_;
		int dataSize_;
		int size_;
	};
}