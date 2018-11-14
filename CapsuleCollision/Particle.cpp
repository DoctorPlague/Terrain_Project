#include "Particle.h"



Particle::Particle(glm::vec3 _position, glm::vec3 _velocity, float _speed, float _lifeTime, short _id)
{
	// Initializing starting values 
	m_position = _position;			// The origin position of the particle
	m_origin = m_position;
	m_velocity = _velocity;			// The direction the particle will move
	m_fSpeed = _speed;				// A multiplier for how much velocity will be applied to the particle
	m_fElapsedTime = _lifeTime;		// How long the particle will exist in it's current context
	m_sID = _id;					// The ID of the particle 
}


Particle::~Particle()
{

}

void Particle::Update(float _deltaTime)
{
	// Update velocity, position and elapsed time
	m_velocity.y += -m_fSpeed * _deltaTime;
	m_position += m_velocity;
	m_fElapsedTime -= _deltaTime;	
	m_velocity = glm::vec3();

	// Reset particle position if necessary
	if (m_fElapsedTime <= 0.0f)
	{
		m_position = m_origin;
		m_fElapsedTime = Random(1.0, 3.0);
	}
}
