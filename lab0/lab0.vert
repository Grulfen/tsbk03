/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 rotMatrix;
uniform float t;

out vec2 ex_TexCoord;
out vec3 ex_Normal;
out vec3 ex_Surface;

mat3 normalMatrix;

void main(void)
{
	normalMatrix = transpose(inverse(mat3(viewMatrix*rotMatrix)));

	ex_TexCoord = in_TexCoord;

	ex_Normal = normalMatrix * in_Normal;
	ex_Surface = vec3(viewMatrix * vec4(in_Position, 1.0));

	gl_Position = projectionMatrix*viewMatrix*rotMatrix*vec4(in_Position, 1.0);
}
