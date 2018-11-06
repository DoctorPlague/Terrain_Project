#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormal;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	fragPos = vec3(model * vec4(position, 1.0f));
	fragNormal = mat3(transpose(inverse(model))) * normal;
	fragTexCoord = texCoord;			
}