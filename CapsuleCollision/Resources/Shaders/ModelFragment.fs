#version 450 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform sampler2D tex;
uniform float ambientStr = 0.0f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos = vec3(400.0f, 100.0f, 300.0f);
uniform float lightSpecStr = 1.0f;
uniform vec3 camPos;// = vec3(1.0f, 10.0f, 600.0f);
uniform float shininess = 1.0f;

void main()
{
	vec3 ambient = ambientStr * ambientColor;
	// Light Direction
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(FragPos - lightPos);
	// Diffuse Coloring
	float diffuseStr = max(dot(norm, -lightDir ), 0.0f);
	vec3 diffuse = diffuseStr * lightColor;
	// Specular Highlight
	vec3 negViewDir = normalize(camPos - FragPos);
	vec3 halfwayVec = normalize(-lightDir + negViewDir);
	float spec = pow(max(dot(norm , halfwayVec), 0.0f), shininess);
	vec3 specular = lightSpecStr * spec * lightColor;

	// Toon Shader stuff
	float intensity = dot(normalize(lightPos), norm);
	vec4 color2;
	vec4 color1 = vec4(diffuse, 1.0f) * (texture(tex, TexCoord));

	if (intensity > 0.95)      color2 = vec4(1.0, 1.0, 1.0, 1.0);
    else if (intensity > 0.75) color2 = vec4(0.8, 0.8, 0.8, 1.0);
    else if (intensity > 0.50) color2 = vec4(0.6, 0.6, 0.6, 1.0);
    else if (intensity > 0.25) color2 = vec4(0.4, 0.4, 0.4, 1.0);
    else                       color2 = vec4(0.2, 0.2, 0.2, 1.0);
	
	color = color1 * color2;


	//color = vec4(ambient + diffuse + specular, 1.0f) * (texture(tex, fragTexCoord));	
	//color = vec4(fragColor, 1.0f) ;
}