#pragma once

#include <string>
#include "Allocator/MemoryLabel.h"

namespace LightningEngine
{
	class FixedString
	{
	public:
		FixedString() : text_(nullptr), len_(0), bufferSize_(0) {}

		FixedString(int len);

		~FixedString();
		void Resize(int len);
		void Set(const char* pStr, int len);
		bool operator==(const char* pText);
		bool operator==(const FixedString& str);
		bool operator==(const std::string& str);
		void operator=(const char* pText);
		void operator=(const FixedString& str);
		void operator=(const std::string& str);
		bool operator!=(const char* pText);
		bool operator!=(const FixedString& str);
		bool operator!=(const std::string& str);
		void operator>>(char* pBuffer);
		void operator<<(const char* pBuffer);

		bool StartWith(const char* pStart);
		bool EndWith(const char* pEnd);
		void TrimEnd(const char* pEnd);
		void TrimEndWithByteCount(int len);
		void TrimExtension();
		void ToSTDPath();
		void ToLuaPath();

	public:
		char* text_;
		int len_;
		int bufferSize_;

	private:
		FixedString(const FixedString& str) {}

	};
}