#version 440 core

out vec4 color;
in float lifetime;

void main()
{
	color = vec4(0.0f, 0.48f, 0.58f, lifetime);
}