#include "Render/LightningEngineGL.h"
#include "Log.h"

#if LIGHTNING_ENGINE_WIN_PLAYER
#pragma comment(lib, "glu32.lib")
#endif


void CheckLastOpenGLError(const char * pFile, long line, const char * operation)
{
	GLenum glerr;
	while ((glerr = glGetError()) != GL_NO_ERROR)
	{
		switch (glerr)
		{
		case GL_INVALID_VALUE:
			Debug("%s GL_INVALID_VALUE", operation);
			break;

		default:
			Debug("gl error 0x%x", (int)glerr);
			break;
		}

#if _DEBUG && LIGHTNING_ENGINE_WIN_PLAYER
		std::string str = pFile;
		const int kMaxErrors = 10;
		int counter = 0;
		int pos = str.find_last_of('\\');
		printf("%s: %ld: ", str.substr(pos + 1, str.length() - pos).c_str(), line);
		const char* gluMsg = (const char*)(gluErrorString(glerr));
		if (gluMsg)
		{
			ErrorPrint("%s %s", operation, gluMsg);
		}
		else
		{
			ErrorPrint("%s 0x%x", operation, glerr);
		}

		++counter;
		if (counter > kMaxErrors)
		{
			Debug("GL: error count exceeds %i, stop reporting errors\n", kMaxErrors);
			return;
		}

#endif
	}
}

void DumpStr(const char * pSrc)
{
	int iLen = strlen(pSrc);
	Debug("dump str len %d", iLen);
	int i = 0;
	while (i < iLen)
	{
		printf("%d %d %d %d %d %d %d %d\n", 
			((*pSrc) & 0x80) >> 7, 
			((*pSrc) & 0x40) >> 6, 
			((*pSrc) & 0x20) >> 5, 
			((*pSrc) & 0x10) >> 4, 
			((*pSrc) & 0x08) >> 3, 
			((*pSrc) & 0x04) >> 2, 
			((*pSrc) & 0x02) >> 1, 
			(*pSrc) & 0x01);
		pSrc++;
		i++;
	}
}

void DumpByte(const char * pSrc, int len)
{
	int i = 0;
	while (i < len)
	{
		printf("%d %d %d %d %d %d %d %d\n", ((*pSrc) & 0x80) >> 7, 
			((*pSrc) & 0x40) >> 6, 
			((*pSrc) & 0x20) >> 5,
			((*pSrc) & 0x10) >> 4,
			((*pSrc) & 0x08) >> 3,
			((*pSrc) & 0x04) >> 2,
			((*pSrc) & 0x02) >> 1,
			(*pSrc) & 0x01);
		pSrc++;
		i++;
	}
}
