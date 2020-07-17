#pragma once

#include "RuntimePrefix.h"
#include "Render/LightningEngineGL.h"
#include "Plugins/Lua/lua.hpp"
#include "EngineData.h"

namespace LightningEngine
{
	struct FileItemNode
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		TCHAR name_[MAX_PATH];
#else
#endif
		bool bIsDir_;
		__int64 lastWriteTime_;
		std::list<FileItemNode*> children_;

		FileItemNode()
		{
#if LIGHTNING_ENGINE_PLATFORM_WIN
			wmemset(name_, 0, MAX_PATH);
#else
#endif
			bIsDir_ = false;
		}

		~FileItemNode()
		{
			if (bIsDir_)
			{
				for (std::list<FileItemNode*>::iterator iter = children_.begin();
					iter != children_.end(); ++iter)
				{
					delete *iter;
				}
			}
		}

	};

	
	class FileSystem
	{
	public:
#if LIGHTNING_ENGINE_PLATFORM_WIN
		static void GetFiles(LPCTSTR path, LPCTSTR filter, FileItemNode& root);
		static void GetFiles(LPCTSTR path, LPCTSTR relativeRootPath, LPCTSTR filter, FileItemNode& root);
		static bool DeleteDir(LPCTSTR path);
		static void GetItemListInDirectory(lua_State* L, const char* path);
#else 
#endif
		static bool SaveData(const char* filePath, Data& data);
		static UInt32 FileSizeOf(const char* path);
		static bool CreateDir(std::string path);
		static bool CreateFile(std::string path);
		static bool MoveFile(const char* srcPath, const char* dstPath);
		static bool DeleteDir(std::string path);
		static bool CopyFile(const char* srcPath, const char* dstPath);
		static bool MoveFileToTrash(const char* path);
		static bool IsDirectoryExist(const std::string& dirPath);
		static bool Exists(const char* path);
		static bool IsAbsolutePath(const std::string& path);
		static char* LoadFile(const char* path);
		static void CopyDir(const char* src, const char* dst);
		static bool LoadDataFromPath(const char* path, Data& data);
		static bool DeleteFileWithPath(std::string path);
	};
}