#include "TessQuad.h"
#include "ShaderLoader.h"
#include "Camera.h"


TessQuad::TessQuad()
{
}


TessQuad::~TessQuad()
{
}

void TessQuad::Initialize(glm::vec3 _t, glm::vec3 _s, glm::vec3 _r, float _ang)
{
	m_program = ShaderLoader::GetInstance().CreateProgram(const_cast<char*>("Resources\\Shaders\\TessVertexShader.vs"),
		const_cast<char*>("Resources\\Shaders\\TessFragmentShader.fs"),
		const_cast<char*>("Resources\\Shaders\\TessControlShader.tcs"),
		const_cast<char*>("Resources\\Shaders\\TessEvaluationShader.tes"));

	// Set variables
	m_translate = _t;
	m_scaling = _s;
	m_rotationAxis = _r;
	m_fangle = _ang;
	m_model = glm::translate(glm::mat4(), m_translate) * glm::rotate(glm::mat4(), glm::radians(m_fangle), m_rotationAxis) * glm::scale(glm::mat4(), m_scaling);

	GLfloat points[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0, 1.0, 0.0f
	};
	glPatchParameteri(GL_PATCH_VERTICES, 4); //comment for tri patch

	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindVertexArray(0);
}

void TessQuad::Render()
{
	glUseProgram(m_program);

	m_model = glm::translate(glm::mat4(), m_translate) * glm::rotate(glm::mat4(), glm::radians(m_fangle), m_rotationAxis) * glm::scale(glm::mat4(), m_scaling);
	glm::mat4 MVP = Camera::GetInstance()->GetProj() * Camera::GetInstance()->GetView() * m_model;
	GLint MVPloc = glGetUniformLocation(m_program, "mvp");
	glUniformMatrix4fv(MVPloc, 1, GL_FALSE, value_ptr(MVP));

	float distanceFloat = glm::distance(m_translate, Camera::GetInstance()->GetPos());
	glUniform1f(glGetUniformLocation(m_program, "distance"), distanceFloat);
	//std::cout << glm::distance(m_translate, Camera::GetInstance()->GetPos()) << std::endl;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		
	glBindVertexArray(m_vao);
	glDrawArrays(GL_PATCHES, 0, 4);
	glBindVertexArray(0);
}
