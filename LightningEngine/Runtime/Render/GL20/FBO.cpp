#include "FBO.h"
#include "Render/RenderState.h"
#include "Debugger/Debugger.h"

namespace LightningEngine
{
	FBO::FBO(GLint fbo) : frameBufferObject_(fbo), 
		width_(512), 
		height_(512), 
		rbo_(nullptr)
	{
		prevFrameBufferObject_ = 0;
		clearColor_.Set(0.1f, 0.4f, 0.6f, 1.0f);
		bSystemFBO_ = !(-1 == fbo);
	}

	FBO::~FBO()
	{
		if (nullptr != rbo_)
		{
			delete rbo_;
			rbo_ = nullptr;
		}

		if (0 != frameBufferObject_ && !bSystemFBO_)
		{
			OGL_CALL(glDeleteFramebuffers(1, &frameBufferObject_));
		}
	}

	void FBO::SetClearColor(float r, float g, float b, float a, int idx)
	{
		clearColor_.Set(r, g, b, a);
	}

	void FBO::Init(int width, int height)
	{
		width_ = width;
		height_ = height;
		colorBuffer_ = new Texture2D;
		colorBuffer_->InitDepthTexture(width, height);
		rbo_ = new RBO;
		rbo_->Init(width, height);
		OGL_CALL(glGenFramebuffers(1, &frameBufferObject_));
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer_->textureID_, 0));
		
#ifdef LIGHTNING_ENGINE_PLATFORM_ANDROID
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
#elif LIGHTNING_ENGINE_PLATFORM_IOS
#else
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
#endif
		int code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (GL_FRAMEBUFFER_COMPLETE != code)
		{
			ErrorPrint("Create fbo fail");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FBO::Resize(int width, int height)
	{
		if (bSystemFBO_)
		{
			width_ = width;
			height_ = height;
			return;
		}

		if (0 == frameBufferObject_)
		{
			return;
		}

		if (nullptr != rbo_)
		{
			DetachDepthStencilRenderBuffer();
			delete rbo_;
			rbo_ = nullptr;
		}

		if (nullptr != colorBuffer_.ptr_)
		{
			DetachColorBuffer(GL_COLOR_ATTACHMENT0);
			colorBuffer_.Release();
		}

		if (nullptr != depthComponentBuffer_.ptr_)
		{
			depthComponentBuffer_.Release();
			OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthComponentBuffer_->textureID_, 0));
			glBindFramebuffer(GL_FRAMEBUFFER, prevFrameBufferObject_);

#ifdef LIGHTNING_ENGINE_PLATFORM_ANDROID
			InitDepthFBOES(width, height);
#elif LIGHTNING_ENGINE_PLATFORM_IOS
#else
			InitDepthFBO(width, height);
#endif
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, prevFrameBufferObject_);
			Init(width, height);
		}
	}

	void FBO::InitDepthFBO(int width, int height)
	{
		width_ = width;
		height_ = height;
		colorBuffer_ = new Texture2D;
		colorBuffer_->InitTexture2D(width, height);
		depthComponentBuffer_ = new Texture2D;
		OGL_CALL(glGenFramebuffers(1, &frameBufferObject_));
		depthComponentBuffer_->InitDepthTexture(width, height);
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer_->textureID_, 0));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthComponentBuffer_->textureID_, 0));
		
		int code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (code != GL_FRAMEBUFFER_COMPLETE)
		{
			InfoPrint("create fbo fail");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FBO::InitDepthFBOES(int width, int height)
	{
		width_ = width;
		height_ = height;
		rbo_ = new RBO;
		rbo_->Init(width, height);
		depthComponentBuffer_ = new Texture2D;
		depthComponentBuffer_->InitTexture2D(width, height);
		OGL_CALL(glGenFramebuffers(1, &frameBufferObject_));
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthComponentBuffer_->textureID_, 0));
#ifdef LIGHTNING_ENGINE_PLATFORM_ANDROID
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
#elif LIGHTNING_ENGINE_PLATFORM_IOS
		
#else
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
#endif
		int code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (code != GL_FRAMEBUFFER_COMPLETE)
		{
			InfoPrint("create fbo fail");
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FBO::InitDepthPartWithDefaultSettings(int width, int height)
	{
		width_ = width;
		height_ = height;
		rbo_ = new RBO;
		rbo_->Init(width, height);
		OGL_CALL(glGenFramebuffers(1, &frameBufferObject_));
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_));

#ifdef LIGHTNING_ENGINE_PLATFORM_ANDROID
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
#elif LIGHTNING_ENGINE_PLATFORM_IOS

#else
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_->renderBufferObject_));
#endif
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FBO::AttachColorBuffer(int attachPoint, Texture2D * pTex)
	{
		width_ = pTex->width_;
		height_ = pTex->height_;
		colorBuffer_ = pTex;
		OGL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_));
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachPoint, GL_TEXTURE_2D, pTex->textureID_, 0));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FBO::DetachColorBuffer(GLenum attachPoint)
	{
		OGL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachPoint, GL_TEXTURE_2D, 0, 0));
	}

	void FBO::DetachRenderBuffer(GLenum attachPoint)
	{
		OGL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachPoint, GL_RENDERBUFFER, 0));
	}

	void FBO::DetachDepthStencilRenderBuffer()
	{
#ifdef LIGHTNING_ENGINE_PLATFORM_ANDROID
		DetachRenderBuffer(GL_DEPTH_ATTACHMENT);
		DetachRenderBuffer(GL_STENCIL_ATTACHMENT);
#elif LIGHTNING_ENGINE_PLATFORM_IOS
#else
		DetachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT);
#endif
	}

	void FBO::Active()
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFrameBufferObject_);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_);
		glViewport(0, 0, width_, height_);
		glClearColor(clearColor_.x_, clearColor_.y_, clearColor_.z_, clearColor_.w_);
		GlobalRenderState::EnableZWrite(true);
		GlobalRenderState::ColorMask(true, true, true, true);
		GlobalRenderState::StencilMask(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void FBO::ActiveDepth()
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFrameBufferObject_);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_);
		glViewport(0, 0, width_, height_);
		glClearColor(clearColor_.x_, clearColor_.y_, clearColor_.z_, clearColor_.w_);
		GlobalRenderState::EnableZWrite(true);
		GlobalRenderState::ColorMask(true, true, true, true);
		GlobalRenderState::StencilMask(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void FBO::Switch()
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFrameBufferObject_);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject_);
	}

	void FBO::Restore()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, prevFrameBufferObject_);
	}


}