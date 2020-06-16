#include "BufferObject.h"
#include "Debugger/Log.h"
#include "Debugger/Debugger.h"

namespace LightningEngine
{
	BufferObject::BufferObject(GLenum type, int size, GLenum hint) : type_(type), identifier_(0)
	{
		if (0 != size)
		{
			Init(type, size, hint);
		}
	}

	BufferObject::~BufferObject()
	{
		if (0 != identifier_)
		{
			OGL_CALL(glDeleteBuffers(1, &identifier_));
		}
	}

	void BufferObject::Init(GLenum type, int size, GLenum hint)
	{
		if (0 != identifier_)
		{
			glDeleteBuffers(1, &identifier_);
		}

		size_ = size;
		OGL_CALL(glGenBuffers(1, &identifier_));
		OGL_CALL(glBindBuffer(type_, identifier_));
		OGL_CALL(glBufferData(type_, size, 0, hint));
		OGL_CALL(glBindBuffer(type_, 0));
	}

	void BufferObject::SubData(const void * pData, int size, int offset)
	{
		if (0 != identifier_)
		{
			if (size > size_)
			{
				ErrorPrint("%u submit data to gpu fail, out of range %d max(%d)", identifier_, size, size_);
				return;
			}

			OGL_CALL(glBindBuffer(type_, identifier_));
			OGL_CALL(glBufferSubData(type_, offset, size, pData));
			OGL_CALL(glBindBuffer(type_, 0));
		}
	}

	bool BufferObject::Active()
	{
		if (0 == identifier_)
		{
			return false;
		}

		OGL_CALL(glBindBuffer(type_, identifier_));
		return true;
	}

	bool BufferObject::Restore()
	{
		if (0 == identifier_)
		{
			return false;
		}
		OGL_CALL(glBindBuffer(type_, 0));
		return true;
	}
}