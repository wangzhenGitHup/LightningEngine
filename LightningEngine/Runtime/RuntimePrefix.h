#pragma once



#define _00000000_00000000_00000000_00000000      1
#define _00000000_00000000_00000000_00000010      2
#define _00000000_00000000_00000000_00000100      4
#define _00000000_00000000_00000000_00001000      8
#define _00000000_00000000_00000000_00010000      16
#define _00000000_00000000_00000000_00100000      32
#define _00000000_00000000_00000000_01000000      64
#define _00000000_00000000_00000000_10000000      128
#define _00000000_00000000_00000001_00000000      256
#define _00000000_00000000_00000010_00000000      512
#define _00000000_00000000_00000100_00000000      1024
#define _00000000_00000000_00001000_00000000      2048
#define _00000000_00000000_00010000_00000000      4096
#define _00000000_00000000_00100000_00000000      8192
#define _00000000_00000000_01000000_00000000      16384
#define _00000000_00000000_10000000_00000000      32768
#define LIGHTNING_ENGINE_DEPRECATED  __declspec(deprecated)

typedef signed char Int8;
typedef unsigned char UInt8;
typedef UInt8 UByte;
typedef Int8  Byte;
typedef signed short Int16;
typedef unsigned short UInt16;
typedef int Int32;
typedef unsigned int UInt32;
typedef signed long long Int64;
typedef unsigned long long UInt64;
typedef Int32 LuaScriptHandle;
typedef void* Any;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <list>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

#define LIGHTNING_ENGINE_MAX_PATH_LEN  260
#define RELEASE (!_DEBUG)

#if LIGHTNING_ENGINE_EDITOR || LIGHTNING_ENGINE_WIN_PLAYER
#define LIGHTNING_ENGINE_PLATFORM_WIN 1
#include <WinSock2.h>
#include <Windows.h>
#include <dwmapi.h>
#include <comdef.h>
#include <gdiplus.h>
#include <ShlObj.h>
#include <WinNls.h>
#include <wchar.h>
#include <tchar.h>
#include <io.h>
#include <direct.h>
#include <ShObjIdl.h>
#include <shellapi.h>
#include <objbase.h>
#include <time.h>
#include<windef.h>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

#define NOMINMAX


#ifndef TCHAR
#define WCHAR TCHAR
#endif

#elif LIGHTNING_ENGINE_IOS || LIGHTNING_ENGINE_ANDROID
#define LIGHTNING_ENGINE_PLATFORM_UNIX 1
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <net/if.h>
#include <pthread.h>
#include <sys/stat.h>
#include <dirent.h>

typedef int64_t __int64;
#endif

#if LIGHTNING_ENGINE_PLATFORM_WIN
#define LIGHTNING_ENGINE_STRICMP  _stricmp
#else
#endif

#if LIGHTNING_ENGINE_EDITOR && _DEBUG
#define OPT(return_type) virtual return type;
#else
#define OPT(return_type) return type;
#endif