#include "RBO.h"
#include "Debugger/Debugger.h"

namespace LightningEngine
{
	RBO::RBO()
	{
		OGL_CALL(glGenRenderbuffers(1, &renderBufferObject_));
	}

	RBO::~RBO()
	{
		OGL_CALL(glDeleteRenderbuffers(1, &renderBufferObject_));
	}

	void RBO::Init(int width, int height)
	{
		OGL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject_));

#ifdef LIGHTNING_ENGINE_PLATFORM_WIN
		OGL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
#endif
		OGL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	}

	void RBO::Resize(int width, int height)
	{
		OGL_CALL(glDeleteRenderbuffers(1, & renderBufferObject_));
		Init(width, height);
	}
}