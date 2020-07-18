#include "FileSystem.h"
#include "String/StringUtils.h"
#include "Debugger/Log.h"

namespace LightningEngine
{
#if LIGHTNING_ENGINE_PLATFORM_WIN
	void FileSystem::GetFiles(LPCTSTR path, LPCTSTR filter, FileItemNode & root)
	{
		TCHAR targetPath[MAX_PATH];
		TCHAR originalWorkingDir[MAX_PATH];
		wmemset(targetPath, 0, MAX_PATH);
		wmemset(originalWorkingDir, 0, MAX_PATH);

		if (path[1] != L':')
		{
			_wgetcwd(targetPath, MAX_PATH);
			lstrcpy(originalWorkingDir, targetPath);
			lstrcat(targetPath, L"\\");
			lstrcat(targetPath, path);
		}

		GetFiles(targetPath, path, filter, root);
		//由于遍历的时候会改变当前的工作目录，所以遍历完了之后要回到原先的工作目录
		_wchdir(originalWorkingDir);
	}


	void FileSystem::GetFiles(LPCTSTR path, LPCTSTR relativePath, LPCTSTR filter, FileItemNode & root)
	{
		TCHAR targetPath[MAX_PATH];
		TCHAR relativeRootPath[MAX_PATH];
		wmemset(relativeRootPath, 0, MAX_PATH);
		lstrcpy(relativeRootPath, relativePath);
		wmemset(targetPath, 0, MAX_PATH);
		if (path[1] != L':')
		{
			_wgetcwd(targetPath, MAX_PATH);
			lstrcat(targetPath, L"\\");
		}

		lstrcat(targetPath, path);
		if ((_waccess(targetPath, 0)) == -1)
		{
			return;
		}

		_wchdir(targetPath);
		long hFile;
		_wfinddata_t fileinfo;
		if ((hFile = _wfindfirst(filter, &fileinfo)) != -1)
		{
			do
			{
				//检查是不是目录
				//如果不是，则进行处理
				if (!(fileinfo.attrib & _A_SUBDIR))
				{
					if (!StringUtils::EndWith(fileinfo.name, L".meta"))
					{
						FileItemNode* pItem = new FileItemNode;
						lstrcpy(pItem->name_, fileinfo.name);
						pItem->bIsDir_ = false;
						pItem->lastWriteTime_ = fileinfo.time_write;
						root.children_.push_back(pItem);
					}
				}
			} while (_wfindnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}

		//查找dir中的子目录
		//因为在处理dir中的文件时，派生类的processFile 有可能改变了
		//当前目录，因此还要重新设置当前目录为dir
		//执行过 _findfirst后，可能系统记录下了相关信息，因此改变目录
		//对 _findnext没有影响
		_wchdir(targetPath);
		if ((hFile = _wfindfirst(filter, &fileinfo)) != -1)
		{
			do
			{
				//检查是不是目录
				//如果是，再检查是不是 . 或 ..
				//如果不是，进行迭代
				if ((fileinfo.attrib & _A_SUBDIR))
				{
					if (lstrcmp(fileinfo.name, L".") != 0 &&
						lstrcmp(fileinfo.name, L"..") != 0 &&
						fileinfo.name[0] != '.')
					{
						TCHAR subdir[MAX_PATH];
						wmemset(subdir, 0, MAX_PATH);
						lstrcpy(subdir, targetPath);
						lstrcat(subdir, L"\\");
						lstrcat(subdir, fileinfo.name);

						lstrcat(relativeRootPath, L"/");
						lstrcat(relativeRootPath, fileinfo.name);

						FileItemNode* pItem = new FileItemNode;
						lstrcpy(pItem->name_, fileinfo.name);
						pItem->bIsDir_ = true;
						pItem->lastWriteTime_ = fileinfo.time_write;
						root.children_.push_back(pItem);

						GetFiles(subdir, relativeRootPath, filter, *pItem);
					}
				}
			} while (_wfindnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

	bool FileSystem::DeleteDir(LPCTSTR path)
	{
		SHFILEOPSTRUCT fileOp;
		fileOp.fFlags = FOF_NOCONFIRMATION | FOF_ALLOWUNDO;
		fileOp.hNameMappings = NULL;
		fileOp.hwnd = NULL;
		fileOp.lpszProgressTitle = NULL;
		fileOp.pFrom = path;
		fileOp.pTo = NULL;
		fileOp.wFunc = FO_DELETE;
		SHFileOperation(&fileOp);
		return true;
	}

	void FileSystem::GetItemListInDirectory(lua_State * L, const char * path)
	{
		long hFile;
		_finddata_t fileinfo;
		int idx = 1;
		char szTemp[256] = { 0 };
		strcpy(szTemp, path);

		if (StringUtils::EndWith(szTemp, "/"))
		{
			strcat(szTemp, "*");
		}
		else
		{
			strcat(szTemp, "/*");
		}

		if ((hFile = _findfirst(szTemp, &fileinfo)) != -1)
		{
			do
			{
				if (fileinfo.name[0] != '.')
				{
					lua_pushinteger(L, idx++);
					lua_newtable(L);
					TCHAR unicode_temp[256] = { 0 };
					char utf8_name[256] = { 0 };
					StringUtils::ASCIIToUnicode(fileinfo.name, unicode_temp);
					StringUtils::UnicodeToUTF8(unicode_temp, utf8_name);
					lua_pushstring(L, utf8_name);
					lua_setfield(L, -2, "name");
					lua_pushboolean(L, (fileinfo.attrib & _A_SUBDIR) != 0);
					lua_setfield(L, -2, "is_dir");
					lua_settable(L, -3);
				}
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}
#else

#endif

	bool FileSystem::SaveData(const char * filePath, Data & data)
	{
		FILE* pFile = fopen(filePath, "wb");
		if (nullptr != pFile)
		{
			if (data.dataLen_ > 0)
			{
				fwrite(data.data_, sizeof(char), data.dataLen_, pFile);
			}

			fclose(pFile);
			return true;
		}

		ErrorPrint("fopen error %d: FileSystem::SaveData cannot open file %s", errno, filePath);
		return false;
	}

	UInt32 FileSystem::FileSizeOf(const char * path)
	{
		UInt32 fileLen = 0;
		FILE* pFile = nullptr;
		if (nullptr != path)
		{
			pFile = fopen(path, "rb");
			if (nullptr != pFile)
			{
				fseek(pFile, 0, SEEK_END);
				fileLen = ftell(pFile);
				fclose(pFile);
			}
		}

		return fileLen;
	}

	bool FileSystem::CreateDir(std::string path)
	{
		size_t start = 0;
		size_t found = path.find_first_of("/\\", start);
		std::string subpath;
		std::vector<std::string> dirs;

		if (found != std::string::npos)
		{
			while (true)
			{
				subpath = path.substr(start, found - start + 1);
				if (!subpath.empty())
				{
					dirs.push_back(subpath);
				}

				start = found + 1;
				found = path.find_first_of("/\\", start);
				if (found == std::string::npos)
				{
					if (start < path.length())
					{
						dirs.push_back(path.substr(start));
					}

					break;
				}
			}
		}
		else
		{
			dirs.push_back(path);
		}

#if LIGHTNING_ENGINE_PLATFORM_WIN
		if ((GetFileAttributesA(path.c_str())) == INVALID_FILE_ATTRIBUTES)
		{
			subpath = "";
			for (unsigned int i = 0; i < dirs.size(); ++i)
			{
				subpath += dirs[i];
				if (!IsDirectoryExist(subpath))
				{
					BOOL ret = CreateDirectoryA(subpath.c_str(), NULL);
					if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
					{
						return false;
					}
				}
			}
		}
		return true;
#else
#endif
		return false;
	}

	bool FileSystem::CreateFile(std::string path)
	{
		size_t found = path.find_last_of("/");
		std::string subpath = path.substr(0, found);
		if (!IsDirectoryExist(subpath))
		{
			CreateDir(subpath);
		}
		std::fstream file(path, std::ios::out);
		file.close();
		return true;
	}

	bool FileSystem::MoveFile(const char * srcPath, const char * dstPath)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		char srcTemp[256];
		char dstTemp[256];
		memset(srcTemp, 0, 256);
		memset(dstTemp, 0, 256);
		strcpy(srcTemp, srcPath);
		strcpy(dstTemp, dstPath);
		LightningEngine::StringUtils::ToWindowsPath(srcTemp);
		LightningEngine::StringUtils::ToWindowsPath(dstTemp);
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_MOVE;
		op.pFrom = srcTemp;
		op.pTo = dstTemp;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_ALLOWUNDO;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		SHFileOperationA(&op);
		return GetLastError() == 0;
#else
#endif
		return false;
	}

	bool FileSystem::DeleteDir(std::string path)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		char szBuffer[256];
		memset(szBuffer, 0, 256);
		strcpy(szBuffer, path.c_str());
		StringUtils::ToSTDPath(szBuffer);
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_DELETE;
		op.pFrom = szBuffer;
		op.pTo = NULL;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_ALLOWUNDO;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		SHFileOperationA(&op);
		return GetLastError() == 0;
#else
#endif
		return false;
	}

	bool FileSystem::CopyFile(const char * srcPath, const char * dstPath)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		char srcTemp[256];
		char dstTemp[256];
		memset(srcTemp, 0, 256);
		memset(dstTemp, 0, 256);
		strcpy(srcTemp, srcPath);
		strcpy(dstTemp, dstPath);
		LightningEngine::StringUtils::ToWindowsPath(srcTemp);
		LightningEngine::StringUtils::ToWindowsPath(dstTemp);
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_COPY;
		op.pFrom = srcTemp;
		op.pTo = dstTemp;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_ALLOWUNDO;
		op.hNameMappings = NULL;
		int retVal = SHFileOperationA(&op);
		return retVal == 0;
#else
#endif

		return false;
	}

	bool FileSystem::MoveFileToTrash(const char * path)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_DELETE;
		op.pFrom = path;
		op.pTo = NULL;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT | FOF_ALLOWUNDO;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		SHFileOperationA(&op);
		return GetLastError() == 0;
#else 
#endif
		return false;
	}

	bool FileSystem::IsDirectoryExist(const std::string & dirPath)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		if (_access(dirPath.c_str(), 0x00) != 0)
#else
#endif
		{
			return false;
		}
		return true;
	}

	bool FileSystem::Exists(const char * path)
	{
		if (nullptr == path)
		{
			return false;
		}

#if LIGHTNING_ENGINE_PLATFORM_WIN
		if (_access(path, 0x00) != 0)
#else 
#endif
		{
			return false;
		}
		return true;
	}

	bool FileSystem::IsAbsolutePath(const std::string & path)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		return (path[1] == ':');
#else
#endif
		return false;
	}

	char * FileSystem::LoadFile(const char * path)
	{
		FILE* pFile = nullptr;
		char* pData = nullptr;
		Int32 count = 0;
		if (nullptr != path)
		{
			pFile = fopen(path, "rb");
			if (nullptr != pFile)
			{
				fseek(pFile, 0, SEEK_END);
				count = ftell(pFile);
				rewind(pFile);
				if (count > 0)
				{
					pData = (char*)malloc(sizeof(char) * (count + 1));
					count = fread(pData, sizeof(char), count, pFile);
					pData[count] = '\0';
				}
				fclose(pFile);
			}
		}

		return pData;
	}

	void FileSystem::CopyDir(const char * src, const char * dst)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		SHFILEOPSTRUCTA sfo;
		ZeroMemory(&sfo, sizeof(sfo));
		char srcPath[_MAX_PATH];
		char dstPath[_MAX_PATH];
		memset(srcPath, 0, _MAX_PATH);
		memset(dstPath, 0, _MAX_PATH);
		strcpy(srcPath, src);
		strcpy(dstPath, dst);
		// \ -> /
		StringUtils::ToWindowsPath(srcPath);
		StringUtils::ToWindowsPath(dstPath);
		strcat(srcPath, "\0\0");
		strcat(dstPath, "\0\0");
		sfo.wFunc = FO_COPY;
		sfo.pFrom = srcPath;// "Assets\0\0";
		sfo.pTo = dstPath;// "IOSbuild/InnerAssets\0";
		sfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
		int ret = SHFileOperationA(&sfo);
		if (0 != ret)
		{
			ErrorPrint("copy file fail %s --> %s", srcPath, dstPath);
		}
#else
#endif
	}

	bool FileSystem::LoadDataFromPath(const char * path, Data & data)
	{
		if (Exists(path))
		{
			FILE* pFile = fopen(path, "rb");
			if (nullptr != pFile)
			{
				fseek(pFile, 0, SEEK_END);
				int file_size = ftell(pFile);
				if (file_size > 0)
				{
					rewind(pFile);
					data.SetBufferSize(file_size + 1);
					data.dataLen_ = fread(data.data_, 1, file_size, pFile);
					data.data_[file_size] = '\0';
					fclose(pFile);
					return true;
				}

				ErrorPrint("fread error : FileSystem::LoadDataFromPath fle %s: data size %ld", path, file_size);
				data.dataLen_ = 0;
				fclose(pFile);
				return false;
			}

			return false;
		}
		return false;
	}

	bool FileSystem::DeleteFileWithPath(std::string path)
	{
#if LIGHTNING_ENGINE_PLATFORM_WIN
		char szBuffer[256];
		memset(szBuffer, 0, 256);
		strcpy(szBuffer, path.c_str());
		StringUtils::ToSTDPath(szBuffer);
		SHFILEOPSTRUCTA op;
		op.hwnd = NULL;
		op.wFunc = FO_DELETE;
		op.pFrom = szBuffer;
		op.pTo = NULL;
		op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
		op.hNameMappings = NULL;
		op.lpszProgressTitle = NULL;
		SHFileOperationA(&op);
		return GetLastError() == 0;
#else
#endif
		return false;
	}
}