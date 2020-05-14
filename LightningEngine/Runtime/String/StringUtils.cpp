#include "StringUtils.h"
#include "Debugger/Log.h"

extern "C" {
#include "External/LibCurl/include/curl/curl.h"
}

#if LIGHTNING_ENGINE_WIN_PLAYER
#pragma comment(lib, "libcurl.lib")
#endif

namespace LightningEngine
{
	int StringUtils::UTF8ToUTF32(const unsigned char * pSrc, int srcLen, UTF32Char * pDst)
	{
		int curSrcPos = 0;
		int i = 0;

		for (; curSrcPos < srcLen;)
		{
			int curLen = UTF8ToUTF32(pSrc + curSrcPos, pDst[i].charCode_);
			pDst[i++].byteCount_ = curLen;

			if (0 == curLen)
			{
				return 0;
			}

			curSrcPos += curLen;
		}

		return i;
	}

	int StringUtils::UTF8ToUTF32(const unsigned char * pSrc, int srcLen, UInt32 * pDst)
	{
		int curSrcPos = 0;
		int i = 0;

		for (; curSrcPos < srcLen;)
		{
			int curLen = UTF8ToUTF32(pSrc + curSrcPos, pDst[i++]);
			if (0 == curLen)
			{
				return 0;
			}

			curSrcPos += curLen;
		}

		return i;
	}

	bool StringUtils::UTF32ToUTF8(const UInt32 * pSrc, int srcLen, unsigned char * pDst)
	{
		return true;
	}

	bool StringUtils::UTF32CharToUTF8(const UInt32 src, unsigned char * pDst, int & len)
	{
		if (src < 0 || src > 0x10FFFF)
		{
			return false;
		}

		if (src < 128)
		{
			pDst[0] = char(src);
			pDst[1] = 0;
			pDst[2] = 0;
			pDst[3] = 0;
			len = 1;
		}
		else if (src < 2048)
		{
			pDst[0] = 0xC0 | char(src >> 6);
			pDst[1] = 0x80 | (char(src) & 0x3F);
			pDst[2] = 0;
			pDst[3] = 0;
			len = 2;
		}
		else if (src < 65536)
		{
			pDst[0] = 0xE0 | char(src >> 12);
			pDst[1] = 0x80 | (char(src >> 6) & 0x3F);
			pDst[2] = 0x80 | (char(src) & 0x3F);
			pDst[3] = 0;
			len = 3;
		}
		else
		{
			pDst[0] = 0xF0 | char(src >> 18);
			pDst[1] = 0x80 | (char(src >> 12) & 0x3F);
			pDst[2] = 0x80 | (char(src >> 6) & 0x3F);
			pDst[3] = 0x80 | (char(src) & 0x3F);
			len = 4;
		}

		return true;
	}

	bool StringUtils::UTF32CharToUTF16(const char32_t u32Ch, Int16 * u16Ch)
	{
		if (u32Ch < 0 || u32Ch > 0x10FFFF)
		{
			return false;
		}

		if (u32Ch < 0x10000)
		{
			u16Ch[0] = char16_t(u32Ch);
			u16Ch[1] = 0;
		}
		else
		{
			u16Ch[0] = char16_t((u32Ch - 0x10000) / 0x400 + 0xD800);
			u16Ch[1] = char16_t((u32Ch - 0x10000) % 0x400 + 0xDC00);
		}
		return true;
	}

	int StringUtils::UTF8CharLen(char ch)
	{
		if (0 <= uint8_t(ch) && uint8_t(ch) < 0x80)
		{
			return 1;
		}

		if (0xC2 <= uint8_t(ch) && uint8_t(ch) < 0xE0)
		{
			return 2;
		}

		if (0xE0 <= uint8_t(ch) && uint8_t(ch) < 0xF0)
		{
			return 3;
		}

		if (0xF0 <= uint8_t(ch) && uint8_t(ch) < 0xF8)
		{
			return 4;
		}

		return 0;
	}

	bool StringUtils::IsUTF8Char(char ch)
	{
		return 0x80 <= uint8_t(ch) && uint8_t(ch) < 0xC0;
	}

	int StringUtils::UTF8ToUTF32(const UInt8 * pSrc, UInt32 & pDst)
	{
		int i, iLen;
		UInt8 b = *pSrc++;

		if (b < 0x80)
		{
			pDst = b;
			return 1;
		}

		//非法UTF8
		if (b < 0xC0 || b > 0xFD)
		{
			return 0;
		}

		if (b < 0xE0)
		{
			pDst = b & 0x1F;
			iLen = 2;
		}
		else if (b < 0xF0)
		{
			pDst = b & 0x0F;
			iLen = 3;
		}
		else if (b < 0xF8)
		{
			pDst = b & 7;
			iLen = 4;
		}
		else if (b < 0xFC)
		{
			pDst = b & 3;
			iLen = 5;
		}
		else
		{
			pDst = b & 1;
			iLen = 6;
		}

		for (i = 1; i < iLen; i++)
		{
			b = *pSrc++;

			//非法UTF8
			if (b < 0x80 || b > 0xBF)
			{
				break;
			}

			pDst = (pDst << 6) + (b & 0x3F);
		}

		//非法UTF8
		if (i < iLen)
		{
			return 0;
		}

		return iLen;
	}

#ifdef _WIN32
	int StringUtils::UnicodeToUTF32(const wchar_t * pSrc, UInt32 * pDst)
	{
		char szTemp[256];
		memset(szTemp, 0, 256);
		UnicodeToUTF8(pSrc, szTemp);
		return UTF8ToUTF32((const unsigned char*)szTemp, strlen(szTemp), pDst);
	}

	void StringUtils::UnicodeToUTF8(const wchar_t * pSrc, char * pDst)
	{
		WideCharToMultiByte(CP_UTF8, 0, pSrc, -1, pDst, 256, NULL, NULL);
	}

	void StringUtils::UnicodeToASCII(const wchar_t * wchar, char * pChr, int len)
	{
		WideCharToMultiByte(CP_ACP, 0, wchar, -1, pChr, len, NULL, NULL);
	}

	void StringUtils::ASCIIToUnicode(const char * pStr, LPTSTR pDst)
	{
		MultiByteToWideChar(CP_ACP, 0, pStr, -1, pDst, 200);
	}

	void StringUtils::UTF8ToUnicode(const char * pStr, LPTSTR pDst)
	{
		MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pDst, 200);
	}

	int StringUtils::GetStringLen(LPTSTR pStr)
	{
		return lstrlen(pStr);
	}
#endif

#if LIGHTNING_ENGINE_WIN_PLAYER || LIGHTNING_ENGINE_EDITOR
	void StringUtils::TrimStart(LPTSTR original, LPCTSTR startToTrim)
	{
		if (wmemcmp(startToTrim, original, lstrlen(startToTrim)) == 0)
		{
			TCHAR szBuffer[256];
			wmemset(szBuffer, 0, 256);
			lstrcpy(szBuffer, original + lstrlen(startToTrim));
			wmemset(original, 0, lstrlen(original));
			lstrcpy(original, szBuffer);
		}
	}

	void StringUtils::TrimEnd(LPTSTR original, LPCTSTR endToTrim)
	{
		wmemset(original + lstrlen(original) - lstrlen(endToTrim), 0, lstrlen(endToTrim));
	}

	bool StringUtils::EndWith(LPCTSTR original, LPCTSTR end)
	{
		if (lstrcmp(original + lstrlen(original) - lstrlen(end), end) == 0)
		{
			return true;
		}

		return false;
	}

	LPCTSTR StringUtils::GetFileExtensionFromPathT(LPCTSTR path)
	{
		size_t len = lstrlen(path);
		LPCTSTR ret = nullptr;
		for (int i = len - 1; i > 0; i--)
		{
			if (path[i] == L'.')
			{
				ret = path + i;
			}
		}

		return ret;
	}

	void StringUtils::TrimStart(char * original, const char * startToTrim)
	{
		if (memcmp(startToTrim, original, strlen(startToTrim)) == 0)
		{
			char szBuffer[256];
			memset(szBuffer, 0, 256);
			strcpy(szBuffer, original + strlen(startToTrim));
			memset(original, 0, strlen(original));
			strcpy(original, szBuffer);
		}
	}

	void StringUtils::TrimEnd(char * original, const char * endToTrim)
	{
		if (EndWith(original, endToTrim))
		{
			memset(original + strlen(original) - strlen(endToTrim), 0, strlen(endToTrim));
		}
	}

	bool StringUtils::EndWith(const char * original, const char * end)
	{
		if (strcmp(original + strlen(original) - strlen(end), end) == 0)
		{
			return true;
		}

		return false;
	}

	bool StringUtils::EndWithI(const char * original, const char * end)
	{
#if LIGHTNING_ENGINE_WIN_PLAYER || LIGHTNING_ENGINE_EDITOR
		if(_stricmp(original + strlen(original) - strlen(end), end) == 0)
#else
		if (strcasecmp(original + strlen(original) - strlen(end), end) == 0)
#endif
		{
			return true;
		}

		return false;
	}

	bool StringUtils::StartWith(const char * original, const char * start)
	{
		if (memcmp(original, start, strlen(start)) == 0)
		{
			return true;
		}

		return false;
	}

	void StringUtils::SplitPathToComponent(char * path, std::vector<std::string>& components)
	{
		ToSTDPath(path);
		int iLen = strlen(path);
		char szTemp[64];
		int iStartPos = 0;

		memset(szTemp, 0, 64);
		for (int i = 0; i < iLen; i++)
		{
			if (path[i] == '/')
			{
				iStartPos = i + 1;
				components.push_back(szTemp);
				memset(szTemp, 0, 64);
			}
			else
			{
				szTemp[i - iStartPos] = path[i];
			}
		}

		components.push_back(szTemp);
	}

	const char * StringUtils::GetFileExtensionFromPath(const char * path)
	{
		size_t len = strlen(path);
		const char* ret = nullptr;
		for (int i = len - 1; i > 0; i--)
		{
			if (path[i] == '.')
			{
				ret = path + i;
				break;
			}
		}
		return ret;
	}

	void StringUtils::GetDirFromFullPath(const char * path, char * dir)
	{
		size_t len = strlen(path);
		for (int i = len - 1; i > 0; i--)
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				memcpy(dir, path, i);
				return;
			}
		}
	}

	void StringUtils::TrimFileExtension(char * path)
	{
		const char* extension = GetFileExtensionFromPath(path);
		if (extension != nullptr)
		{
			TrimEnd(path, extension);
		}
	}

	void StringUtils::GetFileNameWithOutExtension(const char * path, char * name)
	{
		const char* extension = GetFileExtensionFromPath(path);
		size_t len = strlen(path);
		bool bFilledData = false;
		for (int i = len - 1; i >= 0; i--)
		{
			if (path[i] == '/' || path[i] == '\\')
			{
				strcpy(name, path + i + 1);
				bFilledData = true;
				break;
			}
		}

		if (!bFilledData)
		{
			strcpy(name, path);
		}

		if (extension != nullptr)
		{
			TrimEnd(name, extension);
		}
	}

	void StringUtils::ToSTDPath(char * pBuffer)
	{
		size_t iCount = strlen(pBuffer);
		for (size_t i = 0; i < iCount; i++)
		{
			if (pBuffer[i] == '\\')
			{
				pBuffer[i] = '/';
			}
		}
	}

	void StringUtils::ToLuaPath(char * pPath)
	{
		int iLen = strlen(pPath);
		for (int i = 0; i < iLen; i++)
		{
			if (pPath[i] == '/' || pPath[i] == '\\')
			{
				pPath[i] = '.';
			}
		}
	}

	void StringUtils::LuaPathToSTDPath(char * pPath)
	{
		int iLen = strlen(pPath);
		for (int i = 0; i < iLen; i++)
		{
			if (pPath[i] == '.')
			{
				pPath[i] = '/';
			}
		}
	}

	void StringUtils::ToWindowsPath(char * pPath)
	{
		size_t iCount = strlen(pPath);
		for (size_t i = 0; i < iCount; i++)
		{
			if (pPath[i] == '/')
			{
				pPath[i] = '\\';
			}
		}
	}

	int StringUtils::GetNextPosOf(const char * pBuffer, int len, char c)
	{
		for (int i = 0; i < len; i++)
		{
			if (pBuffer[i] == c)
			{
				return i;
			}
		}
		return -1;
	}

	int StringUtils::GetNextPosOfLine(char * pBuffer, int len, char c)
	{
		for (int i = 0; i < len; i++)
		{
			if (pBuffer[i] == c)
			{
				return i;
			}

			if (pBuffer[i] == '\n')
			{
				return i;
			}
		}
		return -1;
	}

	int StringUtils::GetNextPosOfInvert(const char * pBuffer, int len, char c)
	{
		for (int i = len - 1; i >= 0; i--)
		{
			if (pBuffer[i] == c)
			{
				return i;
			}
		}
		return -1;
	}

	void StringUtils::URLEncode(const char * pInput, int len, char * pOutput)
	{
		CURL* curl = curl_easy_init();
		if (curl)
		{
			char* pRet = curl_easy_escape(curl, pInput, len);
			if (pRet)
			{
				strcpy(pOutput, pRet);
				curl_free(pRet);
			}

			curl_easy_cleanup(curl);
		}
	}

	int StringUtils::URLDecode(const char * pInput, int len, char * pOutput)
	{
		CURL* curl = curl_easy_init();
		int outLen = 0;
		if (curl)
		{
			char* pRet = curl_easy_unescape(curl, pInput, len, &outLen);
			if (pRet)
			{
				memcpy(pOutput, pRet, outLen);
				curl_free(pRet);
			}

			curl_easy_cleanup(curl);
		}

		return outLen;
		//return 0;
	}

	bool StringUtils::IsNoneSymbol(char c)
	{
		if (c == '\0')
		{
			return 1;
		}

		int iNum = (int)c;
		return (iNum >= 48 && iNum <= 57) || (iNum >= 65 && iNum <= 90) || (iNum >= 97 && iNum <= 122);
	}

	void StringUtils::BytesToHexStr(const unsigned char * pBytes, int len, char * pBuffer)
	{
		int out_buffer_idx = 0;
		for (int i = 0; i < len; i++)
		{
			sprintf(pBuffer + out_buffer_idx, "%02x", pBytes[i]);
			out_buffer_idx += 2;
		}
	}
#endif
}