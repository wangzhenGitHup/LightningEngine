#pragma once

namespace LightningEngine
{
#if LIGHTNING_ENGINE_ANDROID
#elif LIGHTNING_ENGINE_WIN_PLAYER
	class FileItem
	{
	public:
		char mRelativePath[256];
		__int64 mLastWriteTime;
		static FileItem* Get(const char*path);
	};
#endif
}