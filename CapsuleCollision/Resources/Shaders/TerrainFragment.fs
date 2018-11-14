#version 450 core

in vec2 fragTexCoord;
in vec3 fragPos;
in vec3 fragNormal;
in vec4 FragPosLightSpace;

out vec4 color;

uniform sampler2D shadowMap;
uniform sampler2D tex;
uniform float ambientStr = 0.1f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos;
uniform float lightSpecStr = 1.0f;
uniform vec3 camPos;// = vec3(1.0f, 10.0f, 600.0f);
uniform float shininess = 1.0f;

float ShadowCalculation(vec4 fragPosLightSpace){
	// To get NDC [-1, 1] from screenspace
	vec3 ndcSpace = fragPosLightSpace.xyz/fragPosLightSpace.w;
	// Convert to Tex Coord Space [0,1]
	vec3 texCoordSpace = 0.5f * ndcSpace + 0.5f;
	float bias = .005f;
	float currentDepth = texCoordSpace.z - bias;
	float closestDepth = texture(shadowMap, texCoordSpace.xy).r;
	float shadow = closestDepth > currentDepth ? 0.0 : 1.0;

	return shadow;
}

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
	vec3 color2;
	

	if (intensity > 0.95)      color2 = vec3(1.0, 0.1, 0.1);
    else if (intensity > 0.75) color2 = vec3(0.8, 0.1, 0.1);
    else if (intensity > 0.50) color2 = vec3(0.6, 0.1, 0.1);
    else if (intensity > 0.25) color2 = vec3(0.4, 0.1, 0.1);
    else                       color2 = vec3(0.2, 0.1, 0.1);
	
	float shadow = ShadowCalculation(FragPosLightSpace);
	vec3 color1 = ambient + ((shadow) * (diffuse + specular, 1.0f)); // * (texture(tex, fragTexCoord));
	color = vec4(color1 * color2, 1.0);
	


	//color = vec4(ambient + diffuse + specular, 1.0f) * (texture(tex, fragTexCoord));	
	//color = vec4(fragColor, 1.0f) ;
}