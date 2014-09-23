#version 150

//in vec3 in_Color;
in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
uniform mat4 matrix;

out vec4 g_color;
const vec3 lightDir = normalize(vec3(0.3, 0.5, 1.0));

uniform mat4 g_bones_0_rot;
uniform mat4 g_bones_0_trans;
uniform mat4 g_bones_1_rot;
uniform mat4 g_bones_1_trans;

// Uppgift 3: Soft-skinning på GPU
//
// Flytta över din implementation av soft skinning från CPU-sidan
// till vertexshadern. Mer info finns på hemsidan.

void main(void)
{
	// transformera resultatet med ModelView- och Projection-matriserna
	vec3 tmp_pos_x = vec3(in_TexCoord.x*(g_bones_0_trans*g_bones_0_rot*inverse(g_bones_0_trans))*vec4(in_Position,1));
	vec3 tmp_pos_y = vec3(in_TexCoord.y*(g_bones_1_trans*g_bones_1_rot*inverse(g_bones_1_trans))*vec4(in_Position,1));
	gl_Position = matrix * vec4(tmp_pos_x+tmp_pos_y, 1.0);

	// sätt röd+grön färgkanal till vertex Weights
	vec4 color = vec4(in_TexCoord.x, in_TexCoord.y, 0.0, 1.0);

	// Lägg på en enkel ljussättning på vertexarna 	
	float intensity = dot(in_Normal, lightDir);
	color.xyz *= intensity;

	g_color = color;
}
