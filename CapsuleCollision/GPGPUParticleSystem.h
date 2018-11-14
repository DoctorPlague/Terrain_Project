#pragma once
#include "Utilities.h"

class Camera;
class GPGPUParticle;
class GPGPUParticleSystem
{
public:
	GPGPUParticleSystem();
	~GPGPUParticleSystem();

	void Initialize();
	void Render();

	// Particle Container
	std::vector<std::shared_ptr<GPGPUParticle>> m_vecParticles;
	std::vector<glm::vec3> m_vecPositions;
	std::vector<glm::vec4> m_initialPosition;
	std::vector<glm::vec4> m_initialVelocity;

private:	
	std::shared_ptr<Camera> m_camera; // pointer to camera
	int m_nParticles;				  // the number of particles 
	glm::vec3 m_position;			  // the origin of the particle system
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_texture;
	GLuint m_program;
	GLuint m_computeProgram;
	GLuint m_posVbo, m_velVbo, m_initVelVbo, m_particleVao;
};

