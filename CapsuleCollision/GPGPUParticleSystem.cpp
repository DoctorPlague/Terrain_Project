#include "GPGPUParticleSystem.h"
#include "ShaderLoader.h"
#include "Camera.h"

#define NUM_PARTICLES 128 * 10000


GPGPUParticleSystem::GPGPUParticleSystem()
{
	// Create program
	m_program = ShaderLoader::GetInstance().CreateProgram(const_cast<char*>("Resources\\Shaders\\ComputeVertex.vs"), const_cast<char*>("Resources\\Shaders\\ComputeFragment.fs"));
	m_computeProgram = ShaderLoader::GetInstance().CreateProgram(const_cast<char*>("Resources\\Shaders\\ComputeShader.fs"));
	m_camera = Camera::GetInstance();
}

GPGPUParticleSystem::~GPGPUParticleSystem()
{

}

void GPGPUParticleSystem::Initialize()
{
	// Initialize initial positions and velocity
	m_initialPosition.resize(NUM_PARTICLES);
	m_initialVelocity.resize(NUM_PARTICLES);
	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		m_initialPosition[i] = glm::vec4(0.0f, 0.0f, 0.0f, Random(0.0001, 1.1) + 0.125);//Random(0.0, RAND_MAX) + 0.125);

		double x = 0.25 * cos(i * 0.167 * 0.5) + 0.025f * Random(0.0, 10.0) - 0.125f;
		double y = 2.0f + 0.025f * Random(0.0, 10.0) - 0.125f;
		double z = 0.25 * sin(i * 0.167 * 0.5) + 0.025f * Random(0.0, 10.0) - 0.125f;
		double w = Random(0.0001, 1.1) + 0.125;

		m_initialVelocity[i] = glm::vec4(x,y,z,w);
	}

	// Use a storage buffer to store initial position information
	glGenBuffers(1, &m_posVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_posVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		m_initialPosition.size() * sizeof(glm::vec4),
		&m_initialPosition[0],
		GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_posVbo);

	// Use a storage buffer to store initial velocity values
	glGenBuffers(1, &m_velVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_velVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		m_initialVelocity.size() * sizeof(glm::vec4),
		&m_initialVelocity[0],
		GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_velVbo);
	//
	glGenBuffers(1, &m_initVelVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_initVelVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER,
		m_initialVelocity.size() * sizeof(glm::vec4),
		&m_initialVelocity[0],
		GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_initVelVbo);

	// Create a useless VAO which doesnt get used, but we need it
	glGenVertexArrays(1, &m_particleVao);
	glBindVertexArray(m_particleVao);

	glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GPGPUParticleSystem::Render()
{
	// Use compute program first
	glUseProgram(m_computeProgram);
	glDispatchCompute(NUM_PARTICLES / 128, 1, 1);	
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// Use render program now
	glUseProgram(m_program);

	glm::mat4 vp = m_camera->GetProj() *  m_camera->GetView();
	glUniformMatrix4fv(glGetUniformLocation(m_program, "vp"), 1, GL_FALSE, glm::value_ptr(vp));

	// Bind position buffer as GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, m_posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	glPointSize(2.0f);
	// Render
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	// Tidy up
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);	
}
