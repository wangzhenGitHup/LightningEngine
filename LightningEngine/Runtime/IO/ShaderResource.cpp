#include "ShaderResource.h"
#include "Serializer/GLSL.serializer.h"

namespace LightningEngine
{
	ShaderResource::ShaderResource()
	{
		type_ = ResourceTypeShader;
	}

	ShaderResource * ShaderResource::LoadShader(Serializer::RuntimeAsset * pAsset)
	{
		Serializer::GLSL* pShaderData = new(kMemShaderId)Serializer::GLSL;
		if (pShaderData->ParseFromArray(pAsset->rawdata().c_str(), pAsset->rawdata().length()))
		{
			ShaderResource* pRes = new(kMemShaderId)ShaderResource;
			pRes->resourceData_ = pShaderData;
			return pRes;
		}

		delete pShaderData;
		return nullptr;
	}
}