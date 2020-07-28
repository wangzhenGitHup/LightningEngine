#pragma once

#include "Render/LightningEngineGL.h"
#include "Render/MaterialProperty.h"
#include "ShaderProperties.h"
#include "Serializer/GLSL.serializer.h"
#include "Render/Attribute.h"
#include "Render/RenderMisc.h"
#include <unordered_map>
#include <string>

namespace LightningEngine
{
	class Data;
	class GLSL : public Object
	{
	public:
		GLSL();
		~GLSL();
		static GLuint CompileShader(GLenum type, const char* shaderSrc);
		static GLuint CreateProgram(GLuint vs, GLuint fs, GLuint gs = 0, GLuint tes = 0, GLuint tcs = 0);
		void InitWithSerializedData(Serializer::GLSL* runtimeShader);
		void MakeAsCurrentShader();
		GLint GetAttributeLocation(const char* name);
		GLint GetUniformLocation(const char* name);
		void DefineAttribute(const char* name, int componentCount, GLenum compoentType, bool normalized, int stride, int offset);
		static GLSL* LoadGLSL(const char* name);
		static void UnloadGLSL(const char* name);
		static GLSL* GetCachedProgram(const char* glsl);
		static GLSL* CreateStandardGLSL(const char* vsCode, const char* fsCode);

	public:
		GLuint program_;
		Attribute* attributeHead_;
		static std::unordered_map<std::string, GLSL*> cachedGLSL_;
		static std::unordered_map<std::string, GLSL*> builtinGLSL_;

	};
}