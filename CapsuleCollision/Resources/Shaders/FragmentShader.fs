#version 450 core

in vec2 fragTexCoord;
in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

uniform sampler2D tex;
uniform float ambientStr = 0.1f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos;
uniform float lightSpecStr = 1.0f;
uniform vec3 camPos;// = vec3(1.0f, 10.0f, 600.0f);
uniform float shininess = 1.0f;

void main()
{
	vec3 ambient = ambientStr * ambientColor;
	// Light Direction
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(fragPos - lightPos);
	// Diffuse Coloring
	float diffuseStr = max(dot(norm, -lightDir ), 0.0f);
	vec3 diffuse = diffuseStr * lightColor;
	// Specular Highlight
	vec3 negViewDir = normalize(camPos - fragPos);
	vec3 halfwayVec = normalize(-lightDir + negViewDir);
	float spec = pow(max(dot(norm , halfwayVec), 0.0f), shininess);
	vec3 specular = lightSpecStr * spec * lightColor;

	// Toon Shader stuff
	float intensity = dot(normalize(lightPos), norm);
	vec4 color2;
	vec4 color1 = vec4(ambient + diffuse + specular, 1.0f) ;// * (texture(tex, fragTexCoord));

	if (intensity > 0.95)      color2 = vec4(1.0, 0.1, 0.1, 1.0);
    else if (intensity > 0.75) color2 = vec4(0.8, 0.1, 0.1, 1.0);
    else if (intensity > 0.50) color2 = vec4(0.6, 0.1, 0.1, 1.0);
    else if (intensity > 0.25) color2 = vec4(0.4, 0.1, 0.1, 1.0);
    else                       color2 = vec4(0.2, 0.1, 0.1, 1.0);
	
	color = color1 * color2;


	//color = vec4(ambient + diffuse + specular, 1.0f) * (texture(tex, fragTexCoord));	
	//color = vec4(fragColor, 1.0f) ;
}