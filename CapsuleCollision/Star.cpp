#include "Star.h"
#include "ShaderLoader.h"
#include "Camera.h"


Star::Star()
{
}


Star::~Star()
{
}

void Star::Initialize(glm::vec3 _t, glm::vec3 _s, glm::vec3 _r, float _ang)
{
	m_program = ShaderLoader::GetInstance().CreateProgram(const_cast<char*>("Resources\\Shaders\\StarVertex.vs"), 
		const_cast<char*>("Resources\\Shaders\\StarFragment.fs"),
		const_cast<char*>("Resources\\Shaders\\StarGeometry.gs"));

	// Set variables
	m_translate = _t;
	m_scaling = _s;
	m_rotationAxis = _r;
	m_fangle = _ang;
	m_model = glm::translate(glm::mat4(), m_translate) * glm::rotate(glm::mat4(), glm::radians(m_fangle), m_rotationAxis) * glm::scale(glm::mat4(), m_scaling);

	GLfloat points[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //passing in 1 point
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

void Star::Render()
{
	glUseProgram(m_program);

	m_model = glm::translate(glm::mat4(), m_translate) * glm::rotate(glm::mat4(), glm::radians(m_fangle), m_rotationAxis) * glm::scale(glm::mat4(), m_scaling);
	glm::mat4 MVP = Camera::GetInstance()->GetProj() * Camera::GetInstance()->GetView() * m_model;
	GLint MVPloc = glGetUniformLocation(m_program, "mvp");
	glUniformMatrix4fv(MVPloc, 1, GL_FALSE, value_ptr(MVP));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		
	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);	
}
