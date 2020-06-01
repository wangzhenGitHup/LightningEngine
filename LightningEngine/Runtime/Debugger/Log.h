#pragma once

#define MAX_PATH_FOR_LOG  256
#define MAX_LOG_LENGTH	  1024

int InitEngineLog(const char* engineLogFilePath);
int FormatCurrentDate(char* szTime, char* szDate);
void DebugLog(const char* file, int nLine, const char* format, ...);
void InfoLog(const char* file, int nLine, const char* format, ...);
void ErrorLog(const char* file, int nLine, const char* format, ...);

void SetEngineErrorReporter(void(*func)(const char* msg));
void ReportEngineError(const char* msg);

#if LIGHTNING_ENGINE_PLATFORM_WIN && _DEBUG
#define Debug(f, ...) DebugLog(__FILE__, __LINE__, f, ##__VA_ARGS__)
#else
#define Debug(f, ...)
#endif

#define ErrorPrint(f, ...) ErrorLog(__FILE__, __LINE__, f, ##__VA_ARGS__)
#define InfoPrint(f, ...) InfoLog(__FILE__, __LINE__, f, ##__VA_ARGS__)
