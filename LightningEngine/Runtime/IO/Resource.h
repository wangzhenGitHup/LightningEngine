#pragma once

#include "RuntimePrefix.h"
#include "Base/Object.h"
#include "Plugins/Lua/lua.hpp"
#include "Allocator/TLSFAllocator.h"
#include "EngineData.h"
#include "ResourceType.h"
#include "ResourceManager.h"
#include "Debugger/Log.h"
#include "Serializer/Assets.serializer.h"
#include <unordered_map>


namespace LightningEngine
{
	class Resource : public Object
	{
	public:
		Resource();
		virtual ~Resource();
		void Unload();

		static void Init();
		static bool LoadAssetList(const char* path);
		static void UnloadAssetList(const char* path);
		static void AddResource(const char* name, const char* path, const char* userPath = nullptr);
		static void RemoveResource(const char* name, const char* userPath = nullptr);
		static void Unload(const char* name);
		static Resource* LoadEngineAsset(const char* path, Data& data);
		static Resource* LoadRawData(const char* path, Data& data);
		static Resource* LoadAssetWithUserPath(const char* path);
		static Resource* LoadOuter(const char* path);
		static Resource* LoadAsset(const char* path, Data& data);
		static Serializer::RuntimeAsset* DecodeRuntimeAsset(Data& data);

		template<class T>
		T* RuntimeData()
		{
			return (T*)resourceData_;
		}

		template<class T>
		static T* LoadResource(const char* path)
		{
			if (cachedResources_.find(path) != cachedResources_.end())
			{
				return (T*)cachedResources_[path];
			}

			T* pRet = nullptr;
			Data data;
			if (ResourceManager::LoadData(path, data))
			{
				pRet = (T*)LoadEngineAsset(path, data);
				if (pRet)
				{
					return pRet;
				}

				return (T*)LoadRawData(path, data);
			}

			return pRet;
		}

		DECLEAR_ENGINE_CLASS(Resource)

	public:
		ResourceType type_;
		Serializer::RuntimeAsset* assetBundle_;
		google::protobuf::MessageLite* resourceData_;
		static std::unordered_map<std::string, std::string> assetsIDToPathMap_;
		static std::unordered_map<std::string, std::string> pathToAssetsIDMap_;
		static std::unordered_map<std::string, Resource*> cachedResources_;
		static std::unordered_map<Int32, Resource*(*)(Serializer::RuntimeAsset*)> assetLoader_;
		typedef std::unordered_map<std::string, Resource*>::iterator cachedIter;
	};

	Resource* LoadImageResource(Serializer::RuntimeAsset* asset);
	Resource* LoadBytesResource(Serializer::RuntimeAsset* asset);
}