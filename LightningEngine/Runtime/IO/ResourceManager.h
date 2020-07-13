#pragma once

#include "EngineData.h"
#include "Plugins/Lua/lua.hpp"
#include "String/FixedString.h"


namespace LightningEngine
{
	class ResourceManager
	{
	public:
		static bool Exist(const char* path);
		static bool DeleteFileWithPath(const char* path);
		static bool CreateDir(const char* path);
		static bool DeleteDir(const char* path);
		static void GetItemListInDirectory(lua_State* L, const char* path);
		static void CopyInternalFileOut(const char* internalPath);
		static bool ExistInternal(const char* path);
		static bool LoadInternalData(const char* path, Data& data);
		static bool RenameInternalFileName(const char* srcFile, const char* dstFile);
		static bool DeleteInternalFile(const char* targetFile);
		static bool CopyFileToInternal(const char* targetFile, const char* dstFile);
		static bool MoveInternalFile(const char* srcFile, const char* dstFile);
		static bool SaveInternalData(const char* targetPath, Data& data);
		static bool CreateInternalFile(const char* targetPath);
		static bool LoadOuterData(const char* path, Data& data);
		static bool LoadBuiltinData(const char* path, Data& data);
		static bool LoadData(const char* path, Data& data, FixedString* realPath = nullptr);
		static int UnloadResource(lua_State* L);
		static int AssetsPath(lua_State* L);
		static int DataPath(lua_State* L);
		static int DocPath(lua_State* L);
		static int DesktopPath(lua_State* L);
		static void Init();

	private:
		static bool LoadDataFromRuntimeDataPath(const char* path, Data& data, FixedString* realPath = nullptr);
		static bool LoadDataFromBuiltinDataPath(const char* path, Data& data, FixedString* realPath = nullptr);

	public:
		static FixedString* s_assetsDir;
		static FixedString* s_dataDir;
		static FixedString* s_documentDir;
		static FixedString* s_desktopDir;
	};
}