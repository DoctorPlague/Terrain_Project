#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 fragColor;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);

	if (color != vec3(0.0f, 0.0f, 0.0f))
	{
		fragColor = color;		
	}
	else
	{
		fragColor = vec3(0.0f, 1.0f, 0.0f);
	}
	
}