#pragma once
#include "Utilities.h"

class Particle
{
public:
	Particle(glm::vec3 _position, glm::vec3 _velocity, float _speed, float _lifeTime, short _id);
	~Particle();

	void Update(float _deltaTime);

	float m_fElapsedTime{ 0.0f };
	float m_fSpeed;
	short m_sID;

	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_origin;

};

