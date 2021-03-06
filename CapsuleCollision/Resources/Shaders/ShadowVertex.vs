#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightVPMatrix;
uniform mat4 model;

void main(){
	gl_Position = lightVPMatrix * model * vec4(position, 1.0);
}