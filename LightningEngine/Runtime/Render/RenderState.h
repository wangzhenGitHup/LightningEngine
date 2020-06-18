#pragma once

#include "LightningEngineGL.h"

namespace LightningEngine
{
	struct RenderState
	{
		GLenum primitiveType_;
		bool bBlend_;
		bool bZTest_;
		bool bAlphaTest_;
		bool bWriteR_;
		bool bWriteG_;
		bool bWriteB_;
		bool bWriteA_;
		bool bWriteZ_;
		bool bWriteStencil_;
		bool bCull_;
		bool bEnablePointSprite_;
		int cullFace_;
		int zTestFunction_;
		int alphaTestFunction_;
		float alphaTestValue_;
		float offsetFactor_;
		float offsetUnit_;
		unsigned int srcBlendFunc_;
		unsigned int dstBlendFunc_;
		int clearStencilValue_;
		unsigned int polygonMode_;

		RenderState();
		void operator=(const RenderState& rs);
	};

	class GlobalRenderState
	{
	public:
		static void Init();
		static void BlendFunc(unsigned int src, unsigned int dst);
		static void EnableBlend(bool bEnable);
		static void EnableDepthTest(bool bEnable);
		static void EnablePointSprite(bool bEnable);
		static void EnableRenderTwoSide(bool bEnable);
		static void EnableCull(bool bEnable);
		static void CullFace(int face);
		static void PolygonMode(int mode);
		static bool EnableZWrite(bool bEnable);
		static void ZTest(int func);
		static bool EnableAlphaTest(bool bEnable);
		static void AlphaTest(int func, float v);
		static void ZOffset(float factor, float unit);
		static void ClearColor(float r, float g, float b, float a, int color_buffer_idx = 0);
		static void ColorMask(bool r, bool g, bool b, bool a);
		static void StencilMask(bool bWrite);
		static void ClearStencil(int value);
		static void EnableAttribute(int idx, bool bEnable);
		static void EnableTextureUnit(int idx, bool bEnable = true);
		static void BindTexture(int texType, unsigned int tex);
		
	public:
		static RenderState s_renderState;
		static float s_clearColor[4];
		static bool s_enableAttributes[16];
	};
}