#include "stdafx.h"
#include "Log.h"

#include "RuntimePrefix.h"

char szEngineLog[MAX_PATH_FOR_LOG];

static void(*gEngineErrorReporter)(const char* msg) = nullptr;

int InitEngineLog(const char * engineLogFilePath)
{
	return 0;
}

int FormatCurrentDate(char * szTime, char * szDate)
{
	time_t t;
	time(&t);
	struct tm* today = localtime(&t);
	strftime(szTime, 32, "%H:%M:%S", today);
	strftime(szDate, 32, "%y-%m-%d", today);
	return 0;
}

void DebugLog(const char * file, int nLine, const char * format, ...)
{
}

void InfoLog(const char * file, int nLine, const char * format, ...)
{
}

void ErrorLog(const char * file, int nLine, const char * format, ...)
{
}

void SetEngineErrorReporter(void(*func)(const char* msg))
{
	gEngineErrorReporter = func;
}

void ReportEngineError(const char * msg)
{
	if (gEngineErrorReporter != nullptr)
	{
		gEngineErrorReporter(msg);
	}
	else
	{
		ErrorPrint("%s", msg);
	}
}
