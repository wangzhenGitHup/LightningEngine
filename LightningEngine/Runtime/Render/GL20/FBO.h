#pragma once

#include "RuntimePrefix.h"
#include "Render/LightningEngineGL.h"
#include "Math/Vector4.h"
#include "Utils/SmartPtr.h"
#include "Texture2D.h"
#include "RBO.h"

namespace LightningEngine
{
	struct ColorAttachment
	{
		ColorAttachment()
		{
			attachmentPoint_ = GL_NONE;
			clearColor_.Set(0.1f, 0.4f, 0.6f);
			bClearColorChanged_ = true;
		}

		SmartPtr<Texture2D> colorBufer_;
		GLenum attachmentPoint_;
		Color4 clearColor_;
		bool bClearColorChanged_;
	};

	class FBO
	{
	public:
		FBO(GLint fbo = -1);
		virtual ~FBO();
		void SetClearColor(float r, float g, float b, float a, int idx = 0);
		void Init(int width, int height);
		void Resize(int width, int height);
		void InitDepthFBO(int width, int height);
		void InitDepthFBOES(int width, int height);
		void InitDepthPartWithDefaultSettings(int width, int height);
		void AttachColorBuffer(int attachPoint, Texture2D* pTex);
		void DetachColorBuffer(GLenum attachPoint);
		void DetachRenderBuffer(GLenum attachPoint);
		void DetachDepthStencilRenderBuffer();
		void Active();
		void ActiveDepth();
		void Switch();
		void Restore();

	public:
		int width_, height_;
		GLint prevFrameBufferObject_;
		GLuint frameBufferObject_;
		Color4 clearColor_;
		SmartPtr<Texture2D> colorBuffer_;
		SmartPtr<Texture2D> depthComponentBuffer_;
		RBO* rbo_;
		bool bSystemFBO_;

	};
}