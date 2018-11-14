#pragma once
#include "Utilities.h"


class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	void Initialize();
	void Start();
	void End();

	GLuint GetTexture() { return depthMapTexture; }

private:
	GLuint depthMapFBO; // global variable
	GLuint depthMapTexture;
};

