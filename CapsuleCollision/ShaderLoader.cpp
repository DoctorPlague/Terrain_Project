// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	Jasper Lyons
// Email		:	Jasper.Lyo7552@mediadesign.school.nz
// File Name	:	ShaderLoader.cpp
// Description	:	Implimentation for the ShaderLoader class

#include "ShaderLoader.h" 
#include<iostream>
#include<fstream>
#include<vector>


ShaderLoader* ShaderLoader::s_pInstance = 0;


ShaderLoader::ShaderLoader(void) {}
ShaderLoader::~ShaderLoader(void) {}


ShaderLoader&
ShaderLoader::GetInstance()
{
	if (0 == s_pInstance)
	{
		s_pInstance = new ShaderLoader();
	}
	return *s_pInstance;
}

void
ShaderLoader::DestroyInstance()
{
	if (0 != s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = 0;
	}
}

//Name:			    ReadShader
//Parameters:		char* filename
//Return Type:		std::string
//Description:		Reads the filename and returns a shadercode
//                  
//                  
std::string ShaderLoader::ReadShader(const char *filename)
{
	std::string shaderCode;
	std::ifstream file(filename, std::ios::in);

	if (!file.good()) {
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

//Name:			    CreateShader
//Parameters:		GLenum, string source, char* shadername
//Return Type:		GLuint
//Description:		Creates shader
//                  
//                  
GLuint ShaderLoader::CreateShader(GLenum shaderType, std::string
	source, const char* shaderName)
{

	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = static_cast<int>(source.size());

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE)
	{

		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "ERROR compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}

//Name:			    CreateProgram
//Parameters:		char* vertex shader file, fragment shader file
//Return Type:		GLuint
//Description:		Creates a program with specified shaders
//                  
//                  
GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename,
	const char* fragmentShaderFilename)
{
	std::string _vertexShaderFilename = vertexShaderFilename;
	std::string _fragmentShaderFilename = fragmentShaderFilename;

	GLuint fragment_shader;
	GLuint vertex_shader;

	std::map<std::string, GLuint>::iterator it = VertexShaders.find(_vertexShaderFilename);
	if (it == VertexShaders.end())
	{
		std::string vertex_shader_code = ReadShader(vertexShaderFilename);
		vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
		VertexShaders[_vertexShaderFilename] = vertex_shader;
	}
	else
	{
		vertex_shader = it->second;
	}

	it = FragmentShaders.find(_fragmentShaderFilename);
	if (it == FragmentShaders.end())
	{
		std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
		fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
		FragmentShaders[_fragmentShaderFilename] = fragment_shader;
	}
	else
	{
		fragment_shader = it->second;
	}

	GLuint program;
	it = Programs.find(_vertexShaderFilename + ":" + _fragmentShaderFilename);
	if (it == Programs.end())
	{
		int link_result = 0;
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &link_result);
		Programs[_vertexShaderFilename + ":" + _fragmentShaderFilename] = program;

		if (link_result == GL_FALSE)
		{

			int info_log_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
			std::vector<char> program_log(info_log_length);
			glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
			std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
			return 0;
		}
		return program;
	}
	else
	{
		int link_result = 0;
		program = it->second;
		return program;
	}
}

GLuint ShaderLoader::CreateProgram(const char * vertexShaderFilename, const char * fragmentShaderFilename, const char * geometryShaderFilename)
{
	std::string _vertexShaderFilename = vertexShaderFilename;
	std::string _fragmentShaderFilename = fragmentShaderFilename;
	std::string _geometryShaderFilename = geometryShaderFilename;

	GLuint fragment_shader;
	GLuint vertex_shader;
	GLuint geometry_shader;

	std::map<std::string, GLuint>::iterator it = VertexShaders.find(_vertexShaderFilename);
	if (it == VertexShaders.end())
	{
		std::string vertex_shader_code = ReadShader(vertexShaderFilename);
		vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
		VertexShaders[_vertexShaderFilename] = vertex_shader;
	}
	else
	{
		vertex_shader = it->second;
	}

	it = FragmentShaders.find(_fragmentShaderFilename);
	if (it == FragmentShaders.end())
	{
		std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
		fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
		FragmentShaders[_fragmentShaderFilename] = fragment_shader;
	}
	else
	{
		fragment_shader = it->second;
	}

	it = GeometryShaders.find(_geometryShaderFilename);
	if (it == GeometryShaders.end())
	{
		std::string geometry_shader_code = ReadShader(geometryShaderFilename);
		geometry_shader = CreateShader(GL_GEOMETRY_SHADER, geometry_shader_code, "geometry shader");
		GeometryShaders[_geometryShaderFilename] = geometry_shader;
	}
	else
	{
		geometry_shader = it->second;
	}

	GLuint program;
	it = Programs.find(_vertexShaderFilename + ":" + _fragmentShaderFilename + ":" + _geometryShaderFilename);
	if (it == Programs.end())
	{
		int link_result = 0;
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, geometry_shader);

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &link_result);
		Programs[_vertexShaderFilename + ":" + _fragmentShaderFilename + ":" + _geometryShaderFilename] = program;

		if (link_result == GL_FALSE)
		{

			int info_log_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
			std::vector<char> program_log(info_log_length);
			glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
			std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
			return 0;
		}
		return program;
	}
	else
	{
		int link_result = 0;
		program = it->second;
		return program;
	}
}

GLuint ShaderLoader::CreateProgram(const char * vertexShaderFilename, const char * fragmentShaderFilename, const char * tessellationControlShaderFilename, const char * tessellationEvaluationShaderFilename)
{
	std::string _vertexShaderFilename = vertexShaderFilename;
	std::string _fragmentShaderFilename = fragmentShaderFilename;
	std::string _tessControlShaderFilename = tessellationControlShaderFilename;
	std::string _tessEvaluationShaderFilename = tessellationEvaluationShaderFilename;

	GLuint fragment_shader;
	GLuint vertex_shader;
	GLuint tessCtrl_shader;
	GLuint tessEval_shader;

	std::map<std::string, GLuint>::iterator it = VertexShaders.find(_vertexShaderFilename);
	if (it == VertexShaders.end())
	{
		std::string vertex_shader_code = ReadShader(vertexShaderFilename);
		vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, "vertex shader");
		VertexShaders[_vertexShaderFilename] = vertex_shader;
	}
	else
	{
		vertex_shader = it->second;
	}

	it = FragmentShaders.find(_fragmentShaderFilename);
	if (it == FragmentShaders.end())
	{
		std::string fragment_shader_code = ReadShader(fragmentShaderFilename);
		fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, "fragment shader");
		FragmentShaders[_fragmentShaderFilename] = fragment_shader;
	}
	else
	{
		fragment_shader = it->second;
	}

	it = TessCtrlShaders.find(_tessControlShaderFilename);
	if (it == TessCtrlShaders.end())
	{
		std::string tessCtrl_shader_code = ReadShader(tessellationControlShaderFilename);
		tessCtrl_shader = CreateShader(GL_TESS_CONTROL_SHADER, tessCtrl_shader_code, "tess Control shader");
		TessCtrlShaders[_tessControlShaderFilename] = tessCtrl_shader;
	}
	else
	{
		tessCtrl_shader = it->second;
	}

	it = TessEvalShaders.find(_tessEvaluationShaderFilename);
	if (it == TessEvalShaders.end())
	{
		std::string tessEval_shader_code = ReadShader(tessellationEvaluationShaderFilename);
		tessEval_shader = CreateShader(GL_TESS_EVALUATION_SHADER, tessEval_shader_code, "tess Evaluation shader");
		TessEvalShaders[_tessEvaluationShaderFilename] = tessEval_shader;
	}
	else
	{
		tessEval_shader = it->second;
	}

	GLuint program;
	it = Programs.find(_vertexShaderFilename + ":" + _fragmentShaderFilename + ":" + _tessControlShaderFilename + ":" + _tessEvaluationShaderFilename);
	if (it == Programs.end())
	{
		int link_result = 0;
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glAttachShader(program, tessCtrl_shader);
		glAttachShader(program, tessEval_shader);

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &link_result);
		Programs[_vertexShaderFilename + ":" + _fragmentShaderFilename + ":" + _tessControlShaderFilename + ":" + _tessEvaluationShaderFilename] = program;

		if (link_result == GL_FALSE)
		{

			int info_log_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
			std::vector<char> program_log(info_log_length);
			glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
			std::cout << "Shader Loader : LINK ERROR" << std::endl << &program_log[0] << std::endl;
			return 0;
		}
		return program;
	}
	else
	{
		int link_result = 0;
		program = it->second;
		return program;
	}
}
