#pragma once

#include "Resource.h"

namespace LightningEngine
{
	class ShaderResource : public Resource
	{
	public:
		ShaderResource();
		static ShaderResource* LoadShader(Serializer::RuntimeAsset* pAsset);
	};
}