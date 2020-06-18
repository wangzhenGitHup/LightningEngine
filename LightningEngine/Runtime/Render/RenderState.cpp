#include "RenderState.h"
#include "Debugger/Debugger.h"
#include "LightningEngineGL.h"

namespace LightningEngine
{
	RenderState GlobalRenderState::s_renderState;
	float GlobalRenderState::s_clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	bool GlobalRenderState::s_enableAttributes[16];


	RenderState::RenderState() : primitiveType_(GL_TRIANGLES),
		bBlend_(false), bZTest_(true), bAlphaTest_(false),
		zTestFunction_ (GL_LEQUAL), bWriteR_(true), bWriteG_(true),
		bWriteB_(true), bWriteZ_(true), bWriteStencil_(false), bCull_(true),
		cullFace_(GL_BACK), bEnablePointSprite_(true), alphaTestFunction_(GL_GREATER), 
		alphaTestValue_(0.0f), offsetFactor_(0.0f), offsetUnit_(0.0f), 
		srcBlendFunc_(GL_SRC_ALPHA), dstBlendFunc_(GL_ONE_MINUS_SRC_ALPHA), 
		clearStencilValue_(-1), polygonMode_(GL_FILL)
	{

	}

	void RenderState::operator=(const RenderState & rs)
	{
		memcpy(this, &rs, sizeof(RenderState));
	}

	void GlobalRenderState::Init()
	{
		OGL_CALL(glDisable(GL_BLEND));
		OGL_CALL(glBlendFunc(s_renderState.srcBlendFunc_, s_renderState.dstBlendFunc_));
		OGL_CALL(glEnable(GL_DEPTH_TEST));
		OGL_CALL(glDepthFunc(GL_LEQUAL));
		OGL_CALL(glDepthMask(true));
		OGL_CALL(glColorMask(true, true, true, true));
		OGL_CALL(glStencilMask(true));
		OGL_CALL(glEnable(GL_CULL_FACE));
		OGL_CALL(glCullFace(s_renderState.cullFace_));
		OGL_CALL(glPolygonOffset(s_renderState.offsetFactor_, s_renderState.offsetUnit_));

#if LIGHTNING_ENGINE_PLATFORM_WIN
		OGL_CALL(glDisable(GL_ALPHA_TEST));
		OGL_CALL(glEnable(GL_POINT_SPRITE));
		OGL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));
		glPolygonMode(GL_FRONT_AND_BACK, s_renderState.polygonMode_);
#endif
		
		for (int idx = 0; idx < 16; idx++)
		{
			s_enableAttributes[idx] = false;
		}
	}

	void GlobalRenderState::BlendFunc(unsigned int src, unsigned int dst)
	{
		if (s_renderState.srcBlendFunc_ != src || 
			s_renderState.dstBlendFunc_ != dst)
		{
			s_renderState.srcBlendFunc_ = src;
			s_renderState.dstBlendFunc_ = dst;
			OGL_CALL(glBlendFunc(s_renderState.srcBlendFunc_, s_renderState.dstBlendFunc_));
		}
	}

	void GlobalRenderState::EnableBlend(bool bEnable)
	{
		if (s_renderState.bBlend_ != bEnable)
		{
			s_renderState.bBlend_ = bEnable;
			if (s_renderState.bBlend_)
			{
				OGL_CALL(glEnable(GL_BLEND));
			}
			else
			{
				OGL_CALL(glDisable(GL_BLEND));
			}
		}
	}

	void GlobalRenderState::EnableDepthTest(bool bEnable)
	{
		if (s_renderState.bZTest_ != bEnable)
		{
			s_renderState.bZTest_ = bEnable;
			if (s_renderState.bZTest_)
			{
				OGL_CALL(glEnable(GL_DEPTH_TEST));
			}
			else
			{
				OGL_CALL(glDisable(GL_DEPTH_TEST));
			}
		}
	}

	void GlobalRenderState::EnablePointSprite(bool bEnable)
	{
		if (s_renderState.bEnablePointSprite_ != bEnable)
		{
			s_renderState.bEnablePointSprite_ = bEnable;
			if (s_renderState.bEnablePointSprite_)
			{
#if LIGHTNING_ENGINE_PLATFORM_WIN
				OGL_CALL(glEnable(GL_POINT_SPRITE));
				OGL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));
#endif
			}
			else
			{
#if LIGHTNING_ENGINE_PLATFORM_WIN
				OGL_CALL(glDisable(GL_POINT_SPRITE));
				OGL_CALL(glDisable(GL_PROGRAM_POINT_SIZE));
#endif
			}
		}
	}

	void GlobalRenderState::EnableRenderTwoSide(bool bEnable)
	{
		if (s_renderState.bCull_ != bEnable)
		{
			s_renderState.bCull_ = bEnable;
			if (s_renderState.bCull_)
			{
				OGL_CALL(glDisable(GL_CULL_FACE));
			}
			else
			{
				OGL_CALL(glEnable(GL_CULL_FACE));
			}
		}
	}

	void GlobalRenderState::EnableCull(bool bEnable)
	{
		if (s_renderState.bCull_ != bEnable)
		{
			s_renderState.bCull_ = bEnable;
			if (s_renderState.bCull_)
			{
				OGL_CALL(glEnable(GL_CULL_FACE));
			}
			else
			{
				OGL_CALL(glDisable(GL_CULL_FACE));
			}
		}
	}

	void GlobalRenderState::CullFace(int face)
	{
		if (s_renderState.cullFace_ != face)
		{
			s_renderState.cullFace_ = face;
			OGL_CALL(glCullFace(s_renderState.cullFace_));
		}
	}

	void GlobalRenderState::PolygonMode(int mode)
	{
		if (s_renderState.polygonMode_ != mode)
		{
			s_renderState.polygonMode_ = mode;
#if LIGHTNING_ENGINE_PLATFORM_WIN
			glPolygonMode(GL_FRONT_AND_BACK, s_renderState.polygonMode_);
#endif
		}
	}

	bool GlobalRenderState::EnableZWrite(bool bEnable)
	{
		bool bOriginal = s_renderState.bWriteZ_;
		if (s_renderState.bWriteZ_ != bEnable)
		{
			s_renderState.bWriteZ_ = bEnable;
			OGL_CALL(glDepthMask(s_renderState.bWriteZ_));
		}
		return bOriginal;
	}

	void GlobalRenderState::ZTest(int func)
	{
		if (s_renderState.zTestFunction_ != func)
		{
			s_renderState.zTestFunction_ = func;
			OGL_CALL(glDepthFunc(s_renderState.zTestFunction_));
		}
	}

	bool GlobalRenderState::EnableAlphaTest(bool bEnable)
	{
		bool bOldState = s_renderState.bAlphaTest_;
		if (s_renderState.bAlphaTest_ != bEnable)
		{
			s_renderState.bAlphaTest_ = bEnable;

#if LIGHTNING_ENGINE_PLATFORM_WIN
			if (bEnable)
			{
				OGL_CALL(glEnable(GL_ALPHA_TEST));
			}
			else
			{
				OGL_CALL(glDisable(GL_ALPHA_TEST));
			}
#endif
		}
		return bOldState;
	}

	void GlobalRenderState::AlphaTest(int func, float val)
	{
		if (s_renderState.alphaTestFunction_ != func || 
			s_renderState.alphaTestValue_ != val)
		{
			s_renderState.alphaTestFunction_ = func;
			s_renderState.alphaTestValue_ = val;
#if LIGHTNING_ENGINE_PLATFORM_WIN
			OGL_CALL(glAlphaFunc(func, val));
#endif
		}
	}

	void GlobalRenderState::ZOffset(float factor, float unit)
	{
		if (s_renderState.offsetFactor_ != factor ||
			s_renderState.offsetUnit_ != unit)
		{
			s_renderState.offsetFactor_ = factor;
			s_renderState.offsetUnit_ = unit;
			OGL_CALL(glPolygonOffset(s_renderState.offsetFactor_, s_renderState.offsetUnit_));
		}
	}

	void GlobalRenderState::ClearColor(float r, float g, float b, float a, int color_buffer_idx)
	{
		if (s_clearColor[0] != r ||
			s_clearColor[1] != g ||
			s_clearColor[2] != b ||
			s_clearColor[3] != a)
		{
			s_clearColor[0] = r;
			s_clearColor[1] = g;
			s_clearColor[2] = b;
			s_clearColor[3] = a;
			OGL_CALL(glClearBufferfv(GL_COLOR, color_buffer_idx, s_clearColor));
		}
	}

	void GlobalRenderState::ColorMask(bool r, bool g, bool b, bool a)
	{
		if (s_renderState.bWriteR_ != r ||
			s_renderState.bWriteG_ != g ||
			s_renderState.bWriteB_ != b ||
			s_renderState.bWriteA_ != a)
		{
			s_renderState.bWriteR_ = r;
			s_renderState.bWriteG_ = g;
			s_renderState.bWriteB_ = b;
			s_renderState.bWriteA_ = a;
			OGL_CALL(glColorMask(r, g, b, a));
		}
	}

	void GlobalRenderState::StencilMask(bool bWrite)
	{
		if (s_renderState.bWriteStencil_ != bWrite)
		{
			s_renderState.bWriteStencil_ = bWrite;
			OGL_CALL(glStencilMask(bWrite));
		}
	}

	void GlobalRenderState::ClearStencil(int value)
	{
		if (s_renderState.clearStencilValue_ != value)
		{
			s_renderState.clearStencilValue_ = value;
			OGL_CALL(glClearStencil(s_renderState.clearStencilValue_));
		}
	}

	void GlobalRenderState::EnableAttribute(int idx, bool bEnable)
	{
		if (s_enableAttributes[idx] != bEnable)
		{
			s_enableAttributes[idx] = bEnable;
			if (bEnable)
			{
				OGL_CALL(glEnableVertexAttribArray(idx));
			}
			else
			{
				OGL_CALL(glDisableVertexAttribArray(idx));
			}
		}
	}

	void GlobalRenderState::EnableTextureUnit(int idx, bool bEnable)
	{
		OGL_CALL(glActiveTexture(GL_TEXTURE0 + idx));
	}

	void GlobalRenderState::BindTexture(int texType, unsigned int tex)
	{
		OGL_CALL(glBindTexture(texType, tex));
	}

}