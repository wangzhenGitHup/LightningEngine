#pragma once

#include "LightningEngineGL.h"

namespace LightningEngine
{
	enum PermitiveType
	{
		kPremitiveTypeTriangle,
		kPremitiveTypeTriangleStrip,
		kPremitiveTypeTriangleFan,
		kPremitiveTypePoints,
		kPremitiveTypeLines,
		kPremitiveTypeLineStrip,
		kPremitiveTypeLineLoop,
		kPremitiveTypeCount
	};

	enum RenderingQueue
	{
		kRenderingQueueSkyBox = 1,
		kRenderingQueueOpaque = 500,
		kRenderingQueueTransparent = 1000,
		kRenderingQueueRenderQueueCount
	};

	enum LightingPass {
		kLightingPassNone = 1,
		kLightingPassOnePass = 2,
		kLightingPassForwardBase = 3,
		kLightingPassForwardAdditive = 4,
		kLightingPassDeffered = 5
	};
	enum RenderArchitecture {
		kRenderArchitectureForward = 1,
		kRenderArchitectureDefered,
		kRenderArchitectureCount
	};
	enum RenderPassCatagory {
		kRenderPassCatagoryNoLit = 1,
		kRenderPassCatagoryOnePass,
		kRenderPassCatagoryForwardBase,
		kRenderPassCatagoryForwardAdd,
		kRenderPassCatagoryShadowCaster,
		kRenderPassCatagoryProjector,
		kRenderPassCatagoryDepthMap,
		kRenderPassCatagoryCount
	};


#define ENGINE_ONE                    GL_ONE
#define ENGINE_ZERO                   GL_ZERO
#define ENGINE_SRC_ALPHA              GL_SRC_ALPHA
#define ENGINE_SRC_COLOR              GL_SRC_COLOR
#define ENGINE_DST_ALPHA              GL_DST_ALPHA
#define ENGINE_DST_COLOR              GL_DST_COLOR
#define ENGINE_ONE_MINUS_SRC_COLOR    GL_ONE_MINUS_SRC_COLOR
#define EINGINE_ONE_MINUS_SRC_ALPHA   GL_ONE_MINUS_SRC_ALPHA
#define ENGINE_ONE_MINUS_DST_COLOR    GL_ONE_MINUS_DST_COLOR
#define ENGINE_ONE_MINUS_DST_ALPHA    GL_ONE_MINUS_DST_ALPHA
}