// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	Jasper Lyons
// Email		:	Jasper.Lyo7552@mediadesign.school.nz
// File Name	:	TextLabel.h
// Description	:	Header file defining the TextLabel class
#ifndef __TEXTLABEL_H__
#define __TEXTLABEL_H__
//	Library includes.
#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <map>
#include <string>
#include <iostream>


//	Local includes.


struct Character
{
	GLuint        TextureID;	// Texture ID 
	glm::ivec2    Size;			// Size of glyph
	glm::ivec2    Bearing;		// Positon of glyph 
	GLuint        Advance;		// How far to move for the next character
};

class TextLabel {
public:
	TextLabel();
	TextLabel(std::string text, std::string font, glm::vec2 pos);
	~TextLabel();

	void Render();
	void SetText(std::string newText) { m_sText = newText; };
	void SetColor(glm::vec3 newColor) { m_vColor = newColor; };
	void SetScale(GLfloat newScale) { m_fScale = newScale; };
	void SetPosition(glm::vec2 newPosition) { m_vPosition = newPosition; };
	const glm::vec2& GetPosition() const;
	const std::string& GetText() const;
	const GLfloat& GetScale() const;

private:
	std::string m_sText;	//	string
	GLfloat m_fScale;	//	float
	glm::vec3 m_vColor;	//	vec3
	glm::vec2 m_vPosition;	//	vec2

	GLuint m_iVAO, m_iVBO, m_iProgram;	//	uint
	std::map<GLchar, Character> m_mCharacters;	//	map
};

#endif