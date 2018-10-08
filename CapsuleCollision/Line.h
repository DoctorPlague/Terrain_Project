#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"

class Line
{
public:
	Line();
	~Line();
	void Initialize();
	void Render();

	glm::vec2 p0 = glm::vec2(0.0f, 0.0f);
	glm::vec2 p1 = glm::vec2(0.0f, 0.0f);

private:
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_program;
};

