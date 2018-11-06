// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	Jasper Lyons
// Email		:	Jasper.Lyo7552@mediadesign.school.nz
// File Name	:	TextLabel.cpp
// Description	:	Implimentation for the TextLabel class

#include "TextLabel.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "Utilities.h"
#include "ShaderLoader.h"

TextLabel::TextLabel()
{
}

//Name:			    TextLabel constructor
//Parameters:		string newtext, string new font, text position
//Return Type:		None
//Description:		TextLabel constructor
//                  
//                  
TextLabel::TextLabel(std::string newText, std::string newFont, glm::vec2 pos)
{	
	m_sText = newText;
	m_vColor = glm::vec3(1.0, 1.0, 1.0);
	m_fScale = 1.0;
	SetPosition(pos);
	
	m_iProgram = ShaderLoader::GetInstance().CreateProgram("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs");
	
	glm::mat4 proj = glm::ortho(0.0f, (GLfloat)util_ksScreenWidth, 0.0f, (GLfloat)util_ksScreenHeight);
	glUseProgram(m_iProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_iProgram, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	// Initiate the font Lib
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	// Each character is called a glyph and Face is the collection of glyphs
	FT_Face face;
	// Load font as face;
	if (FT_New_Face(ft, newFont.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to Load font" << std::endl;
	}
	// Set size to load glyph as
	FT_Set_Pixel_Sizes(face, 0, 48); 
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load the first 128 characters of the ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load the character glyph into face
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// Generate texture for character
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Now to store character for later use
		Character character = { texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		m_mCharacters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we are finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &m_iVAO);
	glBindVertexArray(m_iVAO);

	glGenBuffers(1, &m_iVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);		
}

TextLabel::~TextLabel()
{
	m_mCharacters.clear();
}

//Name:			    Render
//Parameters:		None
//Return Type:		None
//Description:		Renders textlabel
//                  
//                  
void TextLabel::Render()
{
	glm::vec2 textPos = m_vPosition;

	// Enable blending
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate corresponding render state
	glUseProgram(m_iProgram);
	glUniform3f(glGetUniformLocation(m_iProgram, "textColor"), m_vColor.x, m_vColor.y, m_vColor.z);
	glActiveTexture(GL_TEXTURE0);	
	glBindVertexArray(m_iVAO);

	// Iterate through the Characters
	for (std::string::const_iterator c = m_sText.begin(); c != m_sText.end(); c++)
	{
		Character ch = m_mCharacters[*c];

		GLfloat xpos = textPos.x + ch.Bearing.x * m_fScale;
		GLfloat ypos = textPos.y - (ch.Size.y - ch.Bearing.y) * m_fScale;

		GLfloat w = ch.Size.x * m_fScale;
		GLfloat h = ch.Size.y * m_fScale;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },{ xpos + w, ypos + h, 1.0, 0.0 },{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos, ypos + h, 0.0, 0.0 },{ xpos + w, ypos, 1.0, 1.0 },{ xpos, ypos, 0.0, 1.0 },
		};

		// Render the glyph texture over the quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);		

		// Now advance cursors for the next glyph
		textPos.x += (ch.Advance >> 6) * m_fScale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_CULL_FACE);	
}

const glm::vec2& TextLabel::GetPosition() const
{
	return m_vPosition;
}

const std::string& TextLabel::GetText() const
{
	return m_sText;
}

const GLfloat& TextLabel::GetScale() const
{
	return m_fScale;
}
