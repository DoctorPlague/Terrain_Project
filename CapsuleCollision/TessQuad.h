#include "Utilities.h"

#pragma once
class TessQuad
{
public:
	TessQuad();
	~TessQuad();
	void Initialize(glm::vec3 _t, glm::vec3 _s, glm::vec3 _r, float _ang);
	void Render();

	glm::vec3 GetTranslate() { return glm::vec3(m_translate); }
	glm::vec3 GetScaling() { return glm::vec3(m_scaling); }
	glm::vec3 GetRotationAxis() { return glm::vec3(m_rotationAxis); }
	float GetRotation() { return m_fangle; }
	glm::mat4 GetModel() { return m_model; }
	void SetPosition(glm::vec3 _translation) { m_translate = _translation; }

private:
	GLuint m_vbo;
	GLuint m_vao;
	GLuint m_ebo;	
	GLuint m_program;
	glm::mat4 m_model;
	glm::vec3 m_translate;
	glm::vec3 m_scaling;
	glm::vec3 m_rotationAxis;
	float m_fangle;
};

