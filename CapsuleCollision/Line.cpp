#include "Line.h"
#include "ShaderLoader.h"
#include "Utilities.h"

Line::Line()
{
	// Create program
	m_program = ShaderLoader::GetInstance().CreateProgram("Resources\\Shaders\\VertexShader.vs",
		"Resources\\Shaders\\FragmentShader.fs");
}


Line::~Line()
{
}

void Line::Initialize()
{
	float vertices[12] = 
	{
		(2.0f * p0.x) / static_cast<float>(ki_SCREENWIDTH) - 1.0f, 1.0f - (2.0f * p0.y) / static_cast<float>(ki_SCREENHEIGHT), 0.0f, 1.0f, 0.0f, 0.0f,
		(2.0f * p1.x) / static_cast<float>(ki_SCREENWIDTH) - 1.0f, 1.0f - (2.0f * p1.y) / static_cast<float>(ki_SCREENHEIGHT), 0.0f, 1.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(m_vao);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(GLfloat),
		(GLvoid*)0
	);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat))
	);
	glEnableVertexAttribArray(1);
}

void Line::Render()
{
	glUseProgram(m_program);
	glBindVertexArray(m_vao);	
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}
