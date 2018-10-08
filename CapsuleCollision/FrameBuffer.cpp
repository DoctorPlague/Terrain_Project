#include "FrameBuffer.h"
#include "ShaderLoader.h"
#include "clock.h"

FrameBuffer::FrameBuffer()
{
}


FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Initialize()
{		
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// generate texture	
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ki_SCREENWIDTH, ki_SCREENHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ki_SCREENWIDTH, ki_SCREENHEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check frame buffer is created and texture and renderbuffer are attached
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	// cleanup
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//// Create and initialize quad for rendering
	m_program = ShaderLoader::GetInstance().CreateProgram(const_cast<char*>("Resources\\Shaders\\FrameBufferVertexShader.vs"),
		const_cast<char*>("Resources\\Shaders\\FrameBufferFragmentShader.fs"));

	GLfloat m_vertices[20] = {
		// Positions					 // Tex Coords
		-1.0f, 1.0f, 0.0f,	 0.0f, 1.0f, // Top Left
		1.0f, 1.0f, 0.0f,	 1.0f, 1.0f, // Top Right
		1.0f, -1.0f, 0.0f,	 1.0f, 0.0f, // Bottom Right
		-1.0f, -1.0f, 0.0f,	 0.0f, 0.0f, // Bottom Left
	};

	GLuint indices[] = {
		0, 1, 2, // First Triangle
		0, 2, 3 // Second Triangle
	};

	glGenVertexArrays(1, &m_QUADvao);
	glBindVertexArray(m_QUADvao);
	glGenBuffers(1, &m_QUADvbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_QUADvbo);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(m_vertices),
		m_vertices,
		GL_STATIC_DRAW);

	glGenBuffers(1, &m_QUADebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QUADebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indices),
		indices,
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void FrameBuffer::Render()
{
	glUseProgram(m_program);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(m_program, "screenTexture"), 0);	
	glUniform1f(glGetUniformLocation(m_program, "iTime"), CClock::GetInstance()->GetDeltaTick());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindVertexArray(m_QUADvao);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);	

	glUseProgram(0);
}

void FrameBuffer::BindFrameBuffer()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
}

