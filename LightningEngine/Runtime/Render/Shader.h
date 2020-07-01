#pragma once

#include "Base/Object.h"
#include "LightningEngineGL.h"
#include "GL20/GLSL.h"

namespace LightningEngine
{
	struct ShaderUniformDefine
	{
		GLint location_;
		GLenum type_;
		int size_;
	};

	class Shader : public Object
	{
	public:
		Shader();
		virtual ~Shader();
		ShaderUniformDefine* GetActiveUniform(const char* pName);
		void AutoActiveUniform();
		static Shader* GetCached(const char* pName);
		static Shader* CreateStandard(const char* pName, const char* vscode, const char* fscode);
		DECLEAR_ENGINE_CLASS(Shader);
		
	public:
		GLSL* gpuProgram_;
		std::unordered_map<std::string, ShaderUniformDefine*> activeUniforms_;
		static std::unordered_map<std::string, Shader*> cacheShader_;
		
	};
}