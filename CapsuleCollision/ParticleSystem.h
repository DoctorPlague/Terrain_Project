#pragma once
#include "Utilities.h"

class Camera;
class Particle;
class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 _origin, std::string texturePath);
	~ParticleSystem();

	void Initialize();
	void Render(float _deltaTime);

	// Particle Container
	std::vector<std::shared_ptr<Particle>> m_vecParticles;
	std::vector<glm::vec3> m_vecPositions;

private:	
	std::shared_ptr<Camera> m_camera; // pointer to camera
	int m_nParticles;				  // the number of particles 
	glm::vec3 m_position;			  // the origin of the particle system
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_texture;
	GLuint m_program;
};

