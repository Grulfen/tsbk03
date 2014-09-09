/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in vec2 ex_TexCoord;
in vec3 ex_Normal;
in vec3 ex_Surface;

uniform sampler2D tex1;
uniform mat4 viewMatrix;

out vec4 out_Color;

void main(void)
{

	// Light in world coordinates
	vec3 light_dir = normalize(mat3(viewMatrix)*vec3(0, 0, 1));
	const vec3 light_col = vec3(1,0,0);
	float diffuse, specular, shade;
	vec4 color;

	// Diffuse
	diffuse = dot(light_dir, normalize(ex_Normal));
	diffuse = max(0, diffuse);

	// Specular
	vec3 r = reflect(-light_dir, normalize(ex_Normal));
	vec3 v = normalize(-ex_Surface);
	specular = max(dot(r,v), 0);
	if(specular > 0)
		specular = pow(specular, 30);

	shade = 0.7*diffuse+ 1*specular;
	color = shade*texture(tex1, ex_TexCoord);

	out_Color = color*vec4(light_col, 1.0);
}
