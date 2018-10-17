#include "Utilities.h"

#pragma once
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	void Initialize();
	void Render(float _deltaTotal);
	void BindFrameBuffer();

private:	
	GLuint m_program;
	GLuint framebuffer;
	GLuint texColorBuffer;
	GLuint rbo;
	GLuint m_QUADvao;
	GLuint m_QUADvbo;
	GLuint m_QUADebo;
};

