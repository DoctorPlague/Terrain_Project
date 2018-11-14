#pragma once

#include <iostream>
#include <map>
#include <string>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"

class ShaderLoader
{
	private:
		ShaderLoader(void);
		~ShaderLoader(void);

		std::string ReadShader(const char *filename);
		GLuint CreateShader(GLenum shaderType,
			std::string source,
			const char* shaderName);
		static ShaderLoader* s_pInstance;

	public:
		static ShaderLoader& GetInstance();
		static void DestroyInstance();
		GLuint CreateProgram(const char* computeShaderFilename);
		GLuint CreateProgram(const char* VertexShaderFilename,
							 const char* FragmentShaderFilename);
		GLuint CreateProgram(const char* VertexShaderFilename,
							 const char* FragmentShaderFilename,
							 const char* GeometryShaderFilename);
		GLuint CreateProgram(const char* VertexShaderFilename,
							 const char* FragmentShaderFilename,
							 const char* TessellationControlShaderFilename,
							 const char* TessellationEvaluationShaderFilename);

		std::map<std::string, GLuint> VertexShaders;
		std::map<std::string, GLuint> FragmentShaders;
		std::map<std::string, GLuint> GeometryShaders;
		std::map<std::string, GLuint> TessCtrlShaders;
		std::map<std::string, GLuint> TessEvalShaders;
		std::map<std::string, GLuint> ComputeShaders;
		std::map<std::string, GLuint> Programs;
};
