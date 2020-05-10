#include "RuntimePrefix.h"

void CheckLastOpenGLError(const char* pFile, long line, const char* operation);

#if _DEBUG
#define OGL_CALL(op) do{op; CheckLastOpenGLError(__FILE__, __LINE__, #op);}while(0)
#else
#define OGL_CALL(op) op;
#endif

void DumpStr(const char* pSrc);
void DumpByte(const char* pSrc, int len);