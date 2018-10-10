#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 16) out;

out vec2 texCoord;

in VS_GS_VERTEX{
	in vec4 position;
	in vec3 color;
	in mat4 mvp;
}gs_in[];

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main() {

	//outColor = gs_in[0].color;	
	mat4 rotationMat90 = rotationMatrix(vec3(0.0f, 1.0f, 0.0f), 1.5708f);		
	mat4 rotatedMVP90 = gs_in[0].mvp * rotationMat90;

	mat4 rotationMat45 = rotationMatrix(vec3(0.0f, 1.0f, 0.0f), 0.78539f);		
	mat4 rotatedMVP45 = gs_in[0].mvp * rotationMat45;

	mat4 rotationMat135 = rotationMatrix(vec3(0.0f, 1.0f, 0.0f), 2.35619f);		
	mat4 rotatedMVP135 = gs_in[0].mvp * rotationMat135;
				
	if (gs_in[0].color != vec3(0.0f, 0.0f, 0.0f))
	{
		// Quad one (90)
		gl_Position = gs_in[0].position + rotatedMVP90 * vec4(2.0f, 0.0f, 0.0f, 0.0f);		
		texCoord = vec2(1.0f, 1.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP90 * vec4(-2.0f, 0.0f, 0.0f, 0.0f);	
		texCoord = vec2(0.0f, 1.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP90 * vec4(2.0f, 2.0f, 0.0f, 0.0f);	
		texCoord = vec2(1.0f, 0.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP90 * vec4(-2.0f, 2.0f, 0.0f, 0.0f);		
		texCoord = vec2(0.0f, 0.0f);
		EmitVertex();
		EndPrimitive();		

		// Quad two (0)
		gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(2.0f, 0.0f, 0.0f, 0.0f);		
		texCoord = vec2(1.0f, 1.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-2.0f, 0.0f, 0.0f, 0.0f);	
		texCoord = vec2(0.0f, 1.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(2.0f, 2.0f, 0.0f, 0.0f);	
		texCoord = vec2(1.0f, 0.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + gs_in[0].mvp * vec4(-2.0f, 2.0f, 0.0f, 0.0f);		
		texCoord = vec2(0.0f, 0.0f);
		EmitVertex();
		EndPrimitive();

		// Quad three (45)
		gl_Position = gs_in[0].position + rotatedMVP45 * vec4(2.0f, 0.0f, 0.0f, 0.0f);		
		texCoord = vec2(1.0f, 1.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP45 * vec4(-2.0f, 0.0f, 0.0f, 0.0f);	
		texCoord = vec2(0.0f, 1.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP45 * vec4(2.0f, 2.0f, 0.0f, 0.0f);	
		texCoord = vec2(1.0f, 0.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP45 * vec4(-2.0f, 2.0f, 0.0f, 0.0f);		
		texCoord = vec2(0.0f, 0.0f);
		EmitVertex();
		EndPrimitive();	

		// Quad four (135)
		gl_Position = gs_in[0].position + rotatedMVP135 * vec4(2.0f, 0.0f, 0.0f, 0.0f);		
		texCoord = vec2(1.0f, 1.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP135 * vec4(-2.0f, 0.0f, 0.0f, 0.0f);	
		texCoord = vec2(0.0f, 1.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP135 * vec4(2.0f, 2.0f, 0.0f, 0.0f);	
		texCoord = vec2(1.0f, 0.0f);
		EmitVertex();

		gl_Position = gs_in[0].position + rotatedMVP135 * vec4(-2.0f, 2.0f, 0.0f, 0.0f);		
		texCoord = vec2(0.0f, 0.0f);
		EmitVertex();
		EndPrimitive();	
	}			
}