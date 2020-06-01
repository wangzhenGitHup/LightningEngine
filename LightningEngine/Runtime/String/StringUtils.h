#pragma once

#include "RuntimePrefix.h"

namespace LightningEngine
{
	struct UTF32Char
	{
		UInt32 charCode_;
		UInt32 byteCount_;
	};

	class StringUtils
	{
	public:
		static int UTF8ToUTF32(const unsigned char* pSrc, int srcLen, UTF32Char* pDst);
		static int UTF8ToUTF32(const unsigned char* pSrc, int srcLen, UInt32* pDst);
		static bool UTF32ToUTF8(const UInt32* pSrc, int srcLen, unsigned char* pDst);
		static bool UTF32CharToUTF8(const UInt32 src, unsigned char* pDst, int& len);
		static bool UTF32CharToUTF16(const char32_t u32Ch, Int16* u16Ch);
		static int UTF8CharLen(char ch);
		static bool IsUTF8Char(char ch);
		static int UTF8ToUTF32(const UInt8* pSrc, UInt32& pDst);

#ifdef _WIN32
		static int UnicodeToUTF32(const wchar_t* pSrc, UInt32* pDst);
		static void UnicodeToUTF8(const wchar_t* pSrc, char* pDst);
		static void UnicodeToASCII(const wchar_t* wchar, char* pChr, int len);
		static void ASCIIToUnicode(const char* pStr, LPTSTR pDst);
		static void UTF8ToUnicode(const char* pStr, LPTSTR pDst);
		static int GetStringLen(LPTSTR pStr);

		template<class T>
		static bool SortName(const T& lValue, const T& rValue)
		{
			char lName[64];
			char rName[64];
			memset(lName, 0, 64);
			memset(rName, 0, 64);
			StringUtils::UnicodeToASCII(lValue->name_, lName, 64);
			StringUtils::UnicodeToASCII(rValue->name_, rName, 64);
			if (strcmp(lName, rName) < 0)
			{
				return true;
			}

			return false;
		}
#endif

#if LIGHTNING_ENGINE_PLATFORM_WIN || LIGHTNING_ENGINE_EDITOR
		static void TrimStart(LPTSTR original, LPCTSTR startToTrim);
		static void TrimEnd(LPTSTR original, LPCTSTR endToTrim);
		static bool EndWith(LPCTSTR original, LPCTSTR end);
		static LPCTSTR GetFileExtensionFromPathT(LPCTSTR path);
#endif

		static void TrimStart(char* original, const char* startToTrim);
		static void TrimEnd(char* original, const char* endToTrim);
		static bool EndWith(const char* original, const char* end);
		static bool EndWithI(const char* original, const char* end);
		static bool StartWith(const char* original, const char* start);
		static void SplitPathToComponent(char* path, std::vector<std::string>& components);

		static const char* GetFileExtensionFromPath(const char* path);
		static void GetDirFromFullPath(const char* path, char* dir);
		static void TrimFileExtension(char* path);
		static void GetFileNameWithOutExtension(const char* path, char* name);

		static void ToSTDPath(char* pBuffer);
		static void ToLuaPath(char* pPath);
		static void LuaPathToSTDPath(char* pPath);
		static void ToWindowsPath(char* pPath);

		static int GetNextPosOf(const char* pBuffer, int len, char c);
		static int GetNextPosOfLine(char* pBuffer, int len, char c);
		static int GetNextPosOfInvert(const char* pBuffer, int len, char c);
		static void URLEncode(const char* pInput, int len, char* pOutput);
		static int URLDecode(const char* pInput, int len, char* pOutput);
		static bool IsNoneSymbol(char c);
		static void BytesToHexStr(const unsigned char* pBytes, int len, char* pBuffer);
	};
}