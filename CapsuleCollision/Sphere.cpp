#define _USE_MATH_DEFINES
#include "Sphere.h"
#include <cmath>
#include <math.h>
#include "ShaderLoader.h"
#include "Camera.h"

Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}

void Sphere::Initialize(glm::vec3 _t, glm::vec3 _s, glm::vec3 _r, float _ang)
{
	m_program = ShaderLoader::GetInstance().CreateProgram(const_cast<char*>("Resources\\Shaders\\VertexShader.vs"), const_cast<char*>("Resources\\Shaders\\FragmentShader.fs"));

	// Set variables
	m_translate = _t;
	m_scaling = _s;
	m_rotationAxis = _r;
	m_fangle = _ang;
	m_model = glm::translate(glm::mat4(), m_translate) * glm::rotate(glm::mat4(), glm::radians(m_fangle), m_rotationAxis) * glm::scale(glm::mat4(), m_scaling);

	// Generate Sphere Mesh
	float radius = 1.0f;

	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	double phi = 0;
	double theta = 0;

	float vertices[(sections) * (sections)* vertexAttrib];
	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = static_cast<float>(cos(phi) * sin(theta));
			float y = static_cast<float>(cos(theta));
			float z = static_cast<float>(sin(phi) * sin(theta));

			vertices[offset++] = x * radius;
			vertices[offset++] = y * radius;
			vertices[offset++] = z * radius;

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (M_PI / (sections - 1));
		}

		phi += (2 * M_PI) / (sections - 1);
	}


	GLuint indices[(sections) * (sections)* indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);
			indices[offset++] = (i * sections) + (j);

			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
		}
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_indiceCount = sizeof(indices) / sizeof(GLuint);
	m_drawType = GL_TRIANGLES;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

void Sphere::Render()
{
	glUseProgram(m_program);

	m_model = glm::translate(glm::mat4(), m_translate) * glm::rotate(glm::mat4(), glm::radians(m_fangle), m_rotationAxis) * glm::scale(glm::mat4(), m_scaling);
	glm::mat4 MVP = Camera::GetInstance()->GetProj() * Camera::GetInstance()->GetView() * m_model;
	GLint MVPloc = glGetUniformLocation(m_program, "mvp");
	glUniformMatrix4fv(MVPloc, 1, GL_FALSE, value_ptr(MVP));	

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	
	glBindVertexArray(m_vao);
	glDrawElements(m_drawType, m_indiceCount, GL_UNSIGNED_INT, 0);	
	glBindVertexArray(0);
}

void Sphere::SetHeight(float _height)
{
	// Check we're on the terrain	
	m_translate.y = _height + 1.5f;	
}

void Sphere::RelativeTranslation(glm::vec3 _translation, float _deltaTick)
{
	m_translate = m_translate + (_translation * _deltaTick);
	m_translate.x = glm::clamp(m_translate.x, -255.0f, 255.0f);
	m_translate.z = glm::clamp(m_translate.z, -255.0f, 255.0f);
}
