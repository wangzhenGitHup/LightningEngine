#include "Resource.h"
#include "IO/FileSystem.h"
#include "String/StringUtils.h"
#include "Serializer/Assets.serializer.h"
#include "Serializer/TextureAtlas.serializer.h"
#include "Debugger/Debugger.h"
#include "TextureResource.h"

namespace LightningEngine
{
	std::unordered_map<std::string, std::string> Resource::assetsIDToPathMap_;
	std::unordered_map<std::string, std::string> Resource::pathToAssetsIDMap_;

	std::unordered_map<Int32, Resource*(*)(Serializer::RuntimeAsset*)> Resource::assetLoader_;
	
	
	Resource::Resource() : type_(ResourceTypeNone),
		assetBundle_(nullptr),
		resourceData_(nullptr),
		Object(1)
	{
	}

	Resource::~Resource()
	{
		if (resourceData_)
		{
			delete resourceData_;
		}

		if (assetBundle_)
		{
			delete assetBundle_;
		}

		
	}

	void Resource::Unload()
	{
		
	}

	void Resource::Init()
	{
		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypeJPGImage, LoadImageResource));
		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypePNGImage, LoadImageResource));
		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypeMaterial, LoadMaterialResource));
		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypeShader, LoadShaderResource));

		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypePrefab, LoadPrefabResource));
		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypeScene, LoadSceneResource));
		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypeAnimation, LoadAnimationResource));
		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypeBytesData, LoadBytesResource));
		assetLoader_.insert(std::pair<ResourceType, Resource*(*)(Serializer::RuntimeAsset*) >(ResourceTypeScript, LoadScriptResource));
	}

	bool Resource::LoadAssetList(const char * path)
	{
		return false;
	}

	void Resource::UnloadAssetList(const char * path)
	{
	}

	void Resource::AddResource(const char * name, const char * path, const char * userPath)
	{
		std::unordered_map<std::string, std::string>::iterator iter = assetsIDToPathMap_.find(name);
		if (iter == assetsIDToPathMap_.end())
		{
			assetsIDToPathMap_.insert(std::pair<std::string, std::string>(name, path));
		}

		if (userPath)
		{
			std::unordered_map<std::string, std::string>::iterator iter = pathToAssetsIDMap_.find(userPath);
			if (iter == pathToAssetsIDMap_.end())
			{
				pathToAssetsIDMap_.insert(std::pair<std::string, std::string>(userPath, name));
			}
		}
	}

	void Resource::RemoveResource(const char * name, const char * userPath)
	{
		std::unordered_map<std::string, std::string>::iterator iter = assetsIDToPathMap_.find(name);
		if (iter != assetsIDToPathMap_.end())
		{
			assetsIDToPathMap_.erase(iter);
		}

		if (userPath)
		{
			std::unordered_map<std::string, std::string>::iterator iter = pathToAssetsIDMap_.find(userPath);
			if (iter != pathToAssetsIDMap_.end())
			{
				pathToAssetsIDMap_.erase(iter);
			}
		}
	}

	void Resource::Unload(const char * name)
	{
		cachedIter iter = cachedResources_.find(name);
		if (iter != cachedResources_.end())
		{
			Resource* pRes = iter->second;
			cachedResources_.erase(iter);
			delete pRes;
		}
	}

	Resource * Resource::LoadEngineAsset(const char * path, Data & data)
	{
		return nullptr;
	}

	Resource * Resource::LoadRawData(const char * path, Data & data)
	{
		Resource* pRes = nullptr;

		if (StringUtils::EndWithI(path, ".png"))
		{
			pRes = TextureResource::CreateTextureResourceFromRawData(path, data);
		}
		else if (StringUtils::EndWithI(path, ".jpg"))
		{
			pRes = TextureResource::CreateTextureResourceFromRawData(path, data);
		}
		else if (StringUtils::EndWithI(path, ".bmp"))
		{
			pRes = TextureResource::CreateTextureResourceFromRawData(path, data);
		}

		if (pRes)
		{
			cachedResources_.insert(std::pair<std::string, Resource*>(path, pRes));
		}
		return pRes;
	}

	Resource * Resource::LoadAssetWithUserPath(const char * path)
	{
		std::unordered_map<std::string, Resource*>::iterator iter = cachedResources_.find(path);
		if (iter != cachedResources_.end())
		{
			iter->second->retain();
			return iter->second;
		}

		Data data;
		FixedString realPath(256);
		if (ResourceManager::LoadData(path, data, &realPath))
		{
			Serializer::RuntimeAsset* pAsset = new(kMemAssetBundleId)Serializer::RuntimeAsset;
			if (pAsset->ParseFromArray(data.data_, data.dataLen_))
			{
				Resource* pRes = new(kMemDefaultId)Resource;
				pRes->type_ = (ResourceType)pAsset->type();
				pRes->assetBundle_ = pAsset;
				cachedResources_.insert(std::pair<std::string, Resource*>(path, pRes));
				return pRes;
			}

			delete pAsset;
		}
		else
		{
			ErrorPrint("Load file fail %s", path);
		}

		return nullptr;
	}

	static Serializer::RuntimeAsset* DecodeRuntimeAsset(Data& data)
	{
		Serializer::RuntimeAsset* pAsset = new(kMemAssetBundleId)Serializer::RuntimeAsset;
		if (pAsset->ParseFromArray(data.data_, data.dataLen_))
		{
			return pAsset;
		}

		delete pAsset;

		return nullptr;
	}

	Resource * Resource::LoadOuter(const char * path)
	{
		Data data;
		Resource* pRes = nullptr;
		if (ResourceManager::LoadOuterData(path, data))
		{
			if (StringUtils::EndWithI(path, ".asset"))
			{
				pRes = LoadEngineAsset(path, data);
			}
			else
			{
				pRes = LoadRawData(path, data);
			}
		}
		else
		{
			ErrorPrint("Resource::LoadOuter fail %s", path);
		}

		return pRes;
	}

	Resource * Resource::LoadAsset(const char * path, Data & data)
	{
		Serializer::RuntimeAsset* pAsset = DecodeRuntimeAsset(data);
		if (pAsset)
		{
			std::unordered_map<Int32, Resource*(*)(Serializer::RuntimeAsset*)>::iterator iterDecoder = assetLoader_.find(pAsset->type());
			if (iterDecoder != assetLoader_.end())
			{
				Resource* pRes = iterDecoder->second(pAsset);
				if (pRes)
				{
					pRes->assetBundle_ = pAsset;
					cachedResources_.insert(std::pair<std::string, Resource*>(path, pRes));
					return pRes;
				}

				ErrorPrint("cannot decode of %d %s", pAsset->type(), path);
				delete pAsset;
			}
			else
			{
				ErrorPrint("cannot find decoder of %d %s", pAsset->type(), path);
				delete pAsset;
			}
		}
		return nullptr;
	}

	Resource * LoadImageResource(Serializer::RuntimeAsset * asset)
	{
		return TextureResource::LoadTexture(asset);
	}

	Resource * LoadBytesResource(Serializer::RuntimeAsset * asset)
	{
		Resource * pRes = new (kMemDefaultId)Resource;
		pRes->assetBundle_ = asset;
		pRes->type_ = (ResourceType)asset->type();
		return pRes;
	}
}