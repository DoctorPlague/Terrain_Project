
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float dTime;

void main()
{ 
	// distance from center of image, used to adjust blur
	//vec2 uv = fragCoord.xy / iResolution.xy;
	//float d = length(TexCoords - vec2(0.5,0.5));
	
	// blur amount
	float blur = 0.0;	
	//blur = (1.0 + sin(dTime*6.0)) * 0.5;
	//blur *= 1.0 + sin(dTime*16.0) * 0.5;
	//blur = pow(blur, 3.0);
	//blur *= 0.05;
	// reduce blur towards center
	//blur *= d;
	
	// final color
    vec3 col;
    col.r = texture(screenTexture, vec2(TexCoords.x+blur,TexCoords.y)).r;
    col.g = texture(screenTexture, TexCoords ).g;
    col.b = texture(screenTexture, vec2(TexCoords.x-blur,TexCoords.y)).b;
	
	// scanline
	//float scanline = sin(TexCoords.y*800.0)*0.04;
	//col -= scanline;
	
	// vignette
	//col *= 1.0 - d * 0.5;

    FragColor = vec4(col,1.0); //texture(screenTexture, TexCoords);
}