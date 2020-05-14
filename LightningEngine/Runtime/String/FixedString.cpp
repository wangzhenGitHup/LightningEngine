#include "FixedString.h"
#include "StringUtils.h"
#include "Runtime/Allocator/TLSFAllocator.h"

namespace LightningEngine
{
	FixedString::FixedString(int len) : len_(0), bufferSize_(len) {
		text_ = new(kMemDefaultId)char[len];
	}

	FixedString::~FixedString()
	{
		if (text_ != nullptr)
		{
			delete[] text_;
			text_ = nullptr;
		}

		len_ = 0;
		bufferSize_ = 0;
	}

	void FixedString::Resize(int len)
	{
		if (nullptr != text_)
		{
			delete[] text_;
		}

		text_ = new (kMemDefaultId)char[len];
		memset(text_, 0, len);
		bufferSize_ = len;
		len_ = 0;
	}

	void FixedString::Set(const char * pStr, int len)
	{
		if ((len + 1) > bufferSize_)
		{
			Resize(len + 1);
		}

		memcpy(text_, pStr, len);
		len_ = len;
		text_[len] = '\0';
	}

	bool FixedString::operator==(const char * pText)
	{
		return strcmp(pText, text_) == 0;
	}

	bool FixedString::operator==(const FixedString & str)
	{
		return strcmp(str.text_, text_) == 0;
	}

	bool FixedString::operator==(const std::string & str)
	{
		return strcmp(str.c_str(), text_) == 0;
	}

	void FixedString::operator=(const char * pText)
	{
		int len = strlen(pText);
		if (len < bufferSize_)
		{
			memset(text_, 0, bufferSize_);
			strcpy(text_, pText);
			len_ = len;
		}
		else
		{
			memcpy(text_, pText, bufferSize_ - 1);
			len_ = bufferSize_;
			text_[bufferSize_ - 1] = '\0';
		}
	}

	void FixedString::operator=(const FixedString & str)
	{
		*this = str.text_;
	}

	void FixedString::operator=(const std::string & str)
	{
		*this = str.c_str();
	}

	bool FixedString::operator!=(const char * pText)
	{
		return strcmp(pText, text_) != 0;
	}

	bool FixedString::operator!=(const FixedString & str)
	{
		return strcmp(str.text_, text_) != 0;
	}

	bool FixedString::operator!=(const std::string & str)
	{
		return strcmp(str.c_str(), text_) != 0;
	}

	void FixedString::operator>>(char * pBuffer)
	{
		strcpy(pBuffer, text_);
	}

	void FixedString::operator<<(const char * pBuffer)
	{
		int len = strlen(pBuffer);
		if (len + len_ < bufferSize_)
		{
			strcat(text_, pBuffer);
			len_ += len;
		}
		else
		{
			memcpy(text_ + len_, pBuffer, bufferSize_ - 1 - len_);
			len_ = bufferSize_;
			text_[bufferSize_ - 1] = '\0';
		}
	}

	bool FixedString::StartWith(const char * pStart)
	{
		return StringUtils::StartWith(text_, pStart);
	}

	bool FixedString::EndWith(const char * pEnd)
	{
		return StringUtils::EndWith(text_, pEnd);
	}

	void FixedString::TrimEnd(const char * pEnd)
	{
		StringUtils::TrimEnd(text_, pEnd);
	}

	void FixedString::TrimEndWithByteCount(int len)
	{
		if(len <= len_)
		{
			memset(text_ + len_ - len, 0, len);
			len_ -= len;
		}
	}

	void FixedString::TrimExtension()
	{
		StringUtils::TrimFileExtension(text_);
	}

	void FixedString::ToSTDPath()
	{
		StringUtils::ToSTDPath(text_);
	}

	void FixedString::ToLuaPath()
	{
		StringUtils::ToLuaPath(text_);
	}
}