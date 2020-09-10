#include "FileItem.h"
#include "Runtime/String/StringUtils.h"
#include "Runtime/Allocator/TLSFAllocator.h"

#if LIGHTNING_ENGINE_WIN_PLAYER
#include <io.h>
#endif

#if LIGHTNING_ENGINE_ANDROID
#elif LIGHTNING_ENGINE_WIN_PLAYER
namespace LightningEngine
{
	FileItem*FileItem::Get(const char*path)
	{
		FileItem*item = nullptr;
#if LIGHTNING_ENGINE_WIN_PLAYER
		_finddata_t fileinfo;
		long hFile = _findfirst(path, &fileinfo);
		if (hFile)
		{
			item = new (kMemDefaultId)FileItem;
			memset(item->mRelativePath, 0, 256);
			strcpy(item->mRelativePath, path);
			item->mLastWriteTime=fileinfo.time_write;
		}
		_findclose(hFile);
#else
#endif
		return item;
	}
}
#endif
