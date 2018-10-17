#version 430 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D tex;

void main(){

vec4 texColor = texture(tex, texCoord);
if (texColor.a < 0.1)
{
	discard;
}
texColor.rgb *= smoothstep(0.1, 1.0, texColor.a);
color = texColor;

}