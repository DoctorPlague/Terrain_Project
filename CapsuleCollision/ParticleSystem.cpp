#include "ParticleSystem.h"
#include "ShaderLoader.h"
#include "Dependencies\soil\SOIL.h"
#include "Particle.h"
#include "Camera.h"


ParticleSystem::ParticleSystem(glm::vec3 _origin, std::string texturePath)
{
	// Create program
	m_program = ShaderLoader::GetInstance().CreateProgram(const_cast<char*>("Resources\\Shaders\\ParticleVertexShader.vs"), const_cast<char*>("Resources\\Shaders\\ParticleFragmentShader.fs"), const_cast<char*>("Resources\\Shaders\\ParticleGeometryShader.gs"));

	// Generate textures
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	int width, height;
	std::string texture = texturePath;
	unsigned char* image = SOIL_load_image(
		texture.c_str(), // File path/name 
		&width, // Output for the image width
		&height, // Output for the image height
		0, // Output for number of channels
		SOIL_LOAD_RGBA); // Load RGBA values only
	glTexImage2D(
		GL_TEXTURE_2D, // Type of texture
		0, // Mipmap level, 0 for base
		GL_RGBA, // Number of color components in texture
		width, // Width of the texture
		height, // Height of the texture
		0, // This value must be 0
		GL_RGBA, // Format for the pixel data
		GL_UNSIGNED_BYTE, // Data type of the pixel data
		image); // Pointer to image data in memory
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set position
	m_position = _origin;

	// Set number of particles; 
	m_nParticles = 10000;	

	// Get camera pointer
	m_camera = Camera::GetInstance();
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Initialize()
{
	// Initialize particles
	for (int i = 0; i < m_nParticles; i++)
	{
		// Add placeholder position to position vector
		m_vecPositions.push_back(glm::vec3(0.0));

		// Create particle object
		std::shared_ptr<Particle> particle = std::make_shared<Particle>(
			m_position + glm::vec3(Random(-50.0, 50.0), 0.0f, Random(-50.0, 50.0)),		// Particle origin position
			glm::vec3(0.0f, -1.0f, 0.0f),												// Particle velocity direction
			static_cast<float>(Random(10.0, 20.0)),										// Particle speed multiplier
			static_cast<float>(Random(1.0, 3.0)),										// Particle life time 
			i																			// Particle ID
			);

		// Push object to particle vector
		m_vecParticles.push_back(particle);
	}

	// Bind vao
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);	

	// Bind vbo
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vecPositions.size(), &m_vecPositions[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleSystem::Render(float _deltaTime)
{
	// Update each particle
	for (int i = 0; i < m_nParticles; i++) {
		m_vecParticles[i]->Update(_deltaTime); // this should probably have delta time as the param
		m_vecPositions[i] = m_vecParticles[i]->m_position;
	}

	// Stuff for bilboarding
	glm::mat4 viewMat = m_camera->GetView();
	glm::vec3 vQuad1, vQuad2;
	glm::vec3 vView = m_camera->GetFront();
	vView = glm::normalize(vView);
	vQuad1 = glm::cross(vView, m_camera->GetUp());
	vQuad1 = glm::normalize(vQuad1);
	vQuad2 = glm::cross(vView, vQuad1);
	vQuad2 = glm::normalize(vQuad2);
	glm::mat4 vp = m_camera->GetProj() *  m_camera->GetView();

	// Use program
	glUseProgram(m_program);

	// Pass uniforms
	glUniform3f(glGetUniformLocation(m_program, "vQuad1"), vQuad1.x, vQuad1.y, vQuad1.z);
	glUniform3f(glGetUniformLocation(m_program, "vQuad2"), vQuad2.x, vQuad2.y, vQuad2.z);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "vp"), 1, GL_FALSE, glm::value_ptr(vp));

	// Use texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(m_program, "Texture"), 0);	

	// Enable rendering stuff
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind vao and vbo
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vecPositions.size(),
		&m_vecPositions[0], GL_STATIC_DRAW);

	// Draw arrays	
	glDrawArrays(GL_POINTS, 0, m_vecPositions.size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Cleanup	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}
