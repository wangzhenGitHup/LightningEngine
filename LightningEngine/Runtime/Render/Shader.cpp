#include "Shader.h"

namespace LightningEngine
{
	std::unordered_map<std::string, Shader*> Shader::cacheShader_;

	Shader::Shader() : gpuProgram_(nullptr)
	{
	}

	Shader::~Shader()
	{
		if (gpuProgram_ != nullptr)
		{
			delete gpuProgram_;
		}
	}

	ShaderUniformDefine * Shader::GetActiveUniform(const char * pName)
	{
		auto iter = activeUniforms_.find(pName);
		if (iter != activeUniforms_.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	void Shader::AutoActiveUniform()
	{
		GLint active_uniform_cnt = 0;
		glGetProgramiv(gpuProgram_->program_, GL_ACTIVE_UNIFORMS, &active_uniform_cnt);
		char szTmp[256];
		GLint size;
		GLenum type;
		for (int idx = 0; idx < active_uniform_cnt; ++idx)
		{
			memset(szTmp, 0, 256);
			glGetActiveUniform(gpuProgram_->program_, idx, 256, NULL, &size, &type, szTmp);
			ShaderUniformDefine* pDef = new ShaderUniformDefine;
			pDef->location_ = gpuProgram_->GetUniformLocation(szTmp);
			pDef->type_ = type;
			pDef->size_ = size;
			activeUniforms_.insert(std::pair<std::string, ShaderUniformDefine*>(szTmp, pDef));
		}
	}

	Shader * Shader::GetCached(const char * pName)
	{
		auto iter = cacheShader_.find(pName);
		if (iter != cacheShader_.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	Shader * Shader::CreateStandard(const char * pName, 
		const char * vscode, const char * fscode)
	{
		GLSL* glsl = GLSL::CreateStandardGLSL(vscode, fscode);
		if (nullptr == glsl)
		{
			ErrorPrint("Shader::CreateStandard failed %s", pName);
			return nullptr;
		}

		glsl->name_ = pName;
		Shader* pShader = new Shader;
		pShader->name_ = pName;
		pShader->gpuProgram_ = glsl;
		pShader->AutoActiveUniform();
		cacheShader_.insert(std::pair<std::string, Shader*>(pName, pShader));
		return pShader;
	}
}