#include "GLSL.h"
#include "Debugger/Debugger.h"
#include "Render/VertexData.h"
#include "String/StringUtils.h"
#include "IO/ResourceManager.h"
#include <stdlib.h>
#include <sstream>


namespace LightningEngine
{
	std::unordered_map<std::string, GLSL*> GLSL::s_cachedGLSL;
	std::unordered_map<std::string, GLSL*> GLSL::s_builtinGLSL;

	GLSL::GLSL() : Object(1), attributeHead_(nullptr), program_(0)
	{

	}

	GLSL::~GLSL()
	{
		glUseProgram(0);
		glDeleteProgram(program_);
		if (nullptr != attributeHead_)
		{
			delete attributeHead_;
		}
	}

	GLSL* GLSL::GetCachedProgram(const char* glsl)
	{
		GLSL* pRet = nullptr;
		if (s_cachedGLSL.find(glsl) != s_cachedGLSL.end())
		{
			pRet = s_cachedGLSL[glsl];
		}

		return pRet;
	}

	void GLSL::InitWithSerializedData(Serializer::GLSL* runtimeShader)
	{
		GLuint vsShader, fsShader;
		vsShader = CompileShader(GL_VERTEX_SHADER, runtimeShader->vert_code().c_str());
		fsShader = CompileShader(GL_VERTEX_SHADER, runtimeShader->frag_code().c_str());
		program_ = CreateProgram(vsShader, fsShader);
		OGL_CALL(glDeleteShader(vsShader));
		OGL_CALL(glDeleteShader(fsShader));
		
		if (0 == program_)
		{
			ErrorPrint("create program fail: %s\n%s\n", runtimeShader->vert_code().c_str(), runtimeShader->frag_code().c_str());
			return;
		}

		int cnt = runtimeShader->attributes_size();
		for (int i = 0; i < cnt; i++)
		{
			const Serializer::AttributeDefine& attribute = runtimeShader->attributes(i);
			GLint attriPos = glGetAttribLocation(program_, attribute.name().c_str());
			if (-1 != attriPos)
			{
				DefineAttribute(attribute.name().c_str(), attribute.component_count(), 
					(GLenum)attribute.data_type(), attribute.normalized() == 1 ? GL_TRUE : GL_FALSE, 
					attribute.stride(), attribute.offset());
			}
			else
			{
				ErrorPrint("unoptimized attribute %s in shader", attribute.name().c_str());
			}
		}

		if (0 == cnt)
		{
			DefineAttribute("pos", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), 0);
			DefineAttribute("texcoord", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), sizeof(float) * 4);
			DefineAttribute("normal", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), sizeof(float) * 8);
			DefineAttribute("tangent", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), sizeof(float) * 12);
			DefineAttribute("texcoord1", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), sizeof(float) * 16);
		}
	}

	void GLSL::UnloadGLSL(const char* name)
	{
		Resource::Unload(name);
		std::unordered_map<std::string, GLSL*>::iterator iter = s_cachedGLSL.find(name);
		if (iter != s_cachedGLSL.end())
		{
			delete iter->second;
			s_cachedGLSL.erase(iter);
		}
	}

	GLSL* GLSL::CreateStandardGLSL(const char* vsCode, const char* fsCode)
	{
		GLint vs = CompileShader(GL_VERTEX_SHADER, vsCode);
		if (0 == vs)
		{
			return nullptr;
		}

		GLint fs = CompileShader(GL_FRAGMENT_SHADER, fsCode);
		if (0 == fs)
		{
			OGL_CALL(glDeleteShader(vs));
			return nullptr;
		}

		GLuint program = CreateProgram(vs, fs);
		if (0 != vs)
		{
			OGL_CALL(glDeleteShader(vs));
		}

		if (0 != fs)
		{
			OGL_CALL(glDeleteShader(fs));
		}

		if (0 == program)
		{
			return nullptr;
		}

		GLSL* glsl = new(kMemDefaultId)GLSL;
		glsl->program_ = program;
		glsl->DefineAttribute("pos", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), 0);
		glsl->DefineAttribute("texcoord", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), sizeof(float) * 4);
		glsl->DefineAttribute("normal", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), sizeof(float) * 8);
		glsl->DefineAttribute("tangent", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), sizeof(float) * 12);
		glsl->DefineAttribute("texcoord1", 4, GL_FLOAT, GL_FALSE, sizeof(LightningEngine::VertexDataFull), sizeof(float) * 16);
		return glsl;
	}

	GLSL* GLSL::LoadGLSL(const char* name)
	{
		GLSL* glsl = GetCachedProgram(name);
		if (nullptr == glsl)
		{
			ShaderResource* pShaderRes = Resource::LoadResource<ShaderResource>(name);
			if (nullptr != pShaderRes)
			{
				Serializer::GLSL* runtimeShader = pShaderRes->RuntimeData<Serializer::GLSL>();
				glsl = new(kMemGfxDeviceId)GLSL;
				glsl->InitWithSerializedData(runtimeShader);
				if (0 == glsl->program_)
				{
					delete glsl;
					ErrorPrint("fatal GLSL: CreateGLSLWithSerializedData %s", name);
					return nullptr;
				}

				glsl->name_ = name;

				s_cachedGLSL.insert(std::pair<std::string, GLSL*>(name, glsl));
				if (LightningEngine::StringUtils::StartWith(name, "builtin"))
				{
					s_builtinGLSL.insert(std::pair<std::string, GLSL*>(name, glsl));
				}
			}
			else
			{
				ErrorPrint("cannot load shader %s", name);
			}
		}

		return glsl;
	}

	GLuint GLSL::CompileShader(GLenum type, const char* shaderSrc)
	{
		GLuint ret = 0;
		OGL_CALL(ret = glCreateShader(type));
		if (0 == ret)
		{
			return 0;
		}

		OGL_CALL(glShaderSource(ret, 1, &shaderSrc, NULL));
		OGL_CALL(glCompileShader(ret));

		GLint iRet;
		OGL_CALL(glGetShaderiv(ret, GL_COMPILE_STATUS, &iRet));
		if (iRet == GL_FALSE)
		{
			char log[1024] = { 0 };
			GLsizei written;
			OGL_CALL(glGetShaderInfoLog(ret, 1024, &written, log));
			ErrorPrint("shader compile error : %s", log);
			ErrorPrint("shader code %s", shaderSrc);
			OGL_CALL(glDeleteShader(ret));
			return 0;
		}

		return ret;
	}

	GLuint GLSL::CreateProgram(GLuint vs, GLuint fs, 
		GLuint gs, GLuint tes, 
		GLuint tcs)
	{
		GLuint program = 0;
		GLint iRet = 0;
		OGL_CALL(program == glCreateProgram());
		if (0 == program)
		{
			ErrorPrint("cannot create gpu program");
			return program;
		}

		OGL_CALL(glAttachShader(program, vs));
		OGL_CALL(glAttachShader(program, fs));
		if (0 != gs)
		{
			OGL_CALL(glAttachShader(program, gs));
		}

		if (0 != tes)
		{
			OGL_CALL(glAttachShader(program, tes));
		}

		if (0 != tcs)
		{
			OGL_CALL(glAttachShader(program, tcs));
		}

		OGL_CALL(glLinkProgram(program));
		OGL_CALL(glDetachShader(program, vs));
		OGL_CALL(glDetachShader(program, fs));

		if (0 != gs)
		{
			OGL_CALL(glDetachShader(program, gs));
		}

		if (0 != tes)
		{
			OGL_CALL(glDetachShader(program, tes));
		}

		if (0 != tcs)
		{
			OGL_CALL(glDetachShader(program, tcs));
		}

		OGL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &iRet));
		if (iRet == GL_FALSE)
		{
			char log[1024] = { 0 };
			GLsizei written;
			OGL_CALL(glGetProgramInfoLog(program, 1024, &written, log));
			ErrorPrint("GLSL Link Error: %s\n", log);
			OGL_CALL(glDeleteProgram(program));
			program = 0;
		}

		return program;
	}

	void GLSL::MakeAsCurrentShader()
	{
		OGL_CALL(glUseProgram(program_));
		if (nullptr != attributeHead_)
		{
			attributeHead_->Enable();
		}
	}

	GLint GLSL::GetAttributeLocation(const char* name)
	{
		if (0 != program_)
		{
			return glGetAttribLocation(program_, name);
		}

		return -1;
	}

	GLint GLSL::GetUniformLocation(const char* name)
	{
		if (0 != program_)
		{
			return glGetUniformLocation(program_, name);
		}

		return -1;
	}

	void GLSL::DefineAttribute(const char* name, int componentCnt, 
		GLenum compoentType, bool normalized, 
		int stride, int offset)
	{
		int iLoc = GetAttributeLocation(name);
		if (-1 != iLoc)
		{
			Attribute* attri = new(kMemShaderId)Attribute;
			attri->attributeSlot_ = GetAttributeLocation(name);
			attri->dataOffset_ = offset;
			attri->dataType_ = compoentType;
			attri->normalized_ = normalized;
			attri->dataStride_ = stride;
			if (nullptr == attributeHead_)
			{
				attributeHead_ = attri;
			}
			else
			{
				attributeHead_->PushBack(attri);
			}
		}
	}
}