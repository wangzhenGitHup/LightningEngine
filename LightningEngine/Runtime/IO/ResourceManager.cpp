#include "ResourceManager.h"
#include "String/StringUtils.h"
#include "FileSystem.h"
#include "Resource.h"

namespace LightningEngine
{
	FixedString* ResourceManager::s_assetsDir = nullptr;
	FixedString* ResourceManager::s_dataDir = nullptr;
	FixedString* ResourceManager::s_documentDir = nullptr;
	FixedString* ResourceManager::s_desktopDir = nullptr;


	bool ResourceManager::Exist(const char * path)
	{
		return FileSystem::Exists(path);
	}

	bool ResourceManager::DeleteFileWithPath(const char * path)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		return FileSystem::DeleteFileWithPath(path);
#else
#endif
		return false;
	}

	bool ResourceManager::CreateDir(const char * path)
	{
		return FileSystem::CreateDir(path);
	}

	bool ResourceManager::DeleteDir(const char * path)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		return FileSystem::DeleteDir(path);
#else
#endif
		return false;
	}

	void ResourceManager::GetItemListInDirectory(lua_State * L, const char * path)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		FileSystem::GetItemListInDirectory(L, path);
#else
#endif
	}

	void ResourceManager::CopyInternalFileOut(const char * internalPath)
	{
		Data srcData;
		if (srcData.dataLen_ != 0)
		{
			char szFullPath[256];
			memset(szFullPath, 0, 256);
			strcpy(szFullPath, s_dataDir->text_);
			strcat(szFullPath, internalPath);
			FILE* pFile = fopen(szFullPath, "wb");
			if (pFile)
			{
				fwrite(srcData.data_, 1, srcData.dataLen_, pFile);
				fclose(pFile);
			}
		}
	}

	bool ResourceManager::ExistInternal(const char * path)
	{
		char szFullPath[256];
		memset(szFullPath, 0, 256);

#if LIGHTNING_ENGINE_EDITOR
		strcpy(szFullPath, "Assets/");
		strcat(szFullPath, path);
		return FileSystem::Exists(szFullPath);
#endif
		return false;
	}

	bool ResourceManager::LoadInternalData(const char * path, Data & data)
	{
		char szFullPath[256];
		memset(szFullPath, 0, 256);

#if  LIGHTNING_ENGINE_EDITOR
		strcpy(szFullPath, s_assetsDir->text_);
		strcat(szFullPath, path);
		return FileSystem::LoadDataFromPath(szFullPath, data);

#elif LIGHTNING_ENGINE_PLATFORM_WIN
		strcpy(szFullPath, s_assetsDir->text_);
		strcat(szFullPath, path);
		return FileSystem::LoadDataFromPath(szFullPath, data);
#endif

#if _DEBUG
		strcpy(szFullPath, s_assetsDir->text_);
		strcat(szFullPath, path);
		return FileSystem::LoadDataFromPath(szFullPath, data);
#else

#endif
		return false;
	}

	bool ResourceManager::RenameInternalFileName(const char * srcFile, const char * dstFile)
	{
		char srcPath[256], dstPath[256];
		memset(srcPath, 0, 256);
		memset(dstPath, 0, 256);

#if LIGHTNING_ENGINE_EDITOR
		strcpy(srcPath, "Assets/");
		strcpy(dstPath, "Assets/");
		strcat(srcPath, srcFile);
		strcat(dstPath, dstFile);
		return FileSystem::MoveFile(srcPath, dstPath);
#else
#endif
		return false;
	}

	bool ResourceManager::DeleteInternalFile(const char * targetFile)
	{
		char srcPath[256];
		memset(srcPath, 0, 256);

#if LIGHTNING_ENGINE_EDITOR
		strcpy(srcPath, "Assets/");
		strcat(srcPath, targetFile);
		return FileSystem::DeleteDir(srcPath);
#else
#endif
		return false;
	}

	bool ResourceManager::CopyFileToInternal(const char * targetFile, const char * dstFile)
	{
		char dstPath[256];
		memset(dstPath, 0, 256);
#if LIGHTNING_ENGINE_EDITOR
		strcpy(dstPath, "Assets/");
		strcat(dstPath, dstFile);
		return FileSystem::CopyFile(targetFile, dstPath);
#else
#endif
		return false;
	}

	bool ResourceManager::MoveInternalFile(const char * srcFile, const char * dstFile)
	{
		char srcPath[256];
		char dstPath[256];
		memset(srcPath, 0, 256);
		memset(dstPath, 0, 256);

#if LIGHTNING_ENGINE_EDITOR
		strcpy(dstPath, "Assets/");
		strcat(dstPath, dstFile);
		strcpy(srcPath, "Assets/");
		strcat(srcPath, srcFile);
		return FileSystem::MoveFile(srcPath, dstPath);
#else
#endif
		return false;
	}

	bool ResourceManager::SaveInternalData(const char * targetPath, Data & data)
	{
		char szFullPath[256];
		memset(szFullPath, 0, 256);

#if LIGHTNING_ENGINE_EDITOR
		strcpy(szFullPath, "Assets/");
		strcat(szFullPath, targetPath);
		return FileSystem::SaveData(szFullPath, data);
#else
#endif
		return false;
	}

	bool ResourceManager::CreateInternalFile(const char * targetPath)
	{
#if LIGHTNING_ENGINE_EDITOR
		char szFullPath[256];
		memset(szFullPath, 0, 256);
		strcpy(szFullPath, "Assets/");
		strcat(szFullPath, targetPath);
		return FileSystem::CreateFile(szFullPath);
#else
#endif
		return false;
	}

	bool ResourceManager::LoadOuterData(const char * path, Data & data)
	{
		char szFullPath[256] = { 0 };
#if LIGHTNING_ENGINE_PLATFORM_WIN
		strcpy(szFullPath, s_dataDir->text_);
		strcat(szFullPath, path);
		return FileSystem::LoadDataFromPath(szFullPath, data);
#else
#endif
		return false;
	}

	bool ResourceManager::LoadBuiltinData(const char * path, Data & data)
	{
#if LIGHTNING_ENGINE_EDITOR
		return FileSystem::LoadDataFromPath(path, data);
#elif LIGHTNING_ENGINE_PLATFORM_WIN
		char szFullDataPath[256];
		memset(szFullDataPath, 0, 256);
		strcpy(szFullDataPath, "Data/");
		strcat(szFullDataPath, path);
		return FileSystem::LoadDataFromPath(szFullDataPath, data);
#endif
		return false;
	}

	bool ResourceManager::LoadData(const char * path, Data & data, FixedString * realPath)
	{
		if (LoadDataFromRuntimeDataPath(path, data, realPath))
		{
			return true;
		}

		if (LoadDataFromBuiltinDataPath(path, data, realPath))
		{
			return true;
		}

		if (FileSystem::LoadDataFromPath(path, data))
		{
			if (realPath)
			{
				(*realPath) = path;
			}

			return true;
		}
		return false;
	}

	int ResourceManager::UnloadResource(lua_State * L)
	{
		return 0;
	}

	int ResourceManager::AssetsPath(lua_State * L)
	{
		lua_pushstring(L, s_dataDir->text_);
		return 0;
	}

	int ResourceManager::DataPath(lua_State * L)
	{
		lua_pushstring(L, s_documentDir->text_);
		return 0;
	}

	int ResourceManager::DocPath(lua_State * L)
	{
		lua_pushstring(L, s_documentDir->text_);
		return 0;
	}

	int ResourceManager::DesktopPath(lua_State * L)
	{
		lua_pushstring(L, s_desktopDir->text_);
		return 0;
	}

	void ResourceManager::Init()
	{
		s_assetsDir = new(kMemDefaultId)FixedString(256);
		s_dataDir = new(kMemDefaultId)FixedString(256);
		s_documentDir = new(kMemDefaultId)FixedString(256);
		s_desktopDir = new(kMemDefaultId)FixedString(256);

#if LIGHTNING_ENGINE_EDITOR
		(*s_assetsDir) = "Assets/";
		(*s_dataDir) = "Data/";
		(*s_documentDir) = "";
		(*s_desktopDir) = "Code/";

#elif LIGHTNING_ENGINE_PLATFORM_WIN
		(*s_assetsDir) = "Assets/";
		(*s_dataDir) = "Data/";
		(*s_documentDir) = "";
		(*s_desktopDir) = "Code/";
#endif
	}

	bool ResourceManager::LoadDataFromRuntimeDataPath(const char * path, Data & data, FixedString * realPath)
	{
		return false;
	}

	bool ResourceManager::LoadDataFromBuiltinDataPath(const char * path, Data & data, FixedString * realPath)
	{
		return false;
	}

}