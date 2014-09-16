#version 150

in vec2 outTexCoord;
uniform sampler2D texUnit;
out vec4 out_Color;

uniform float t;

float gauss_coeff[25] = float[]( 1, 4, 6, 4,1,
			4,16,24,16,4,
			6,24,36,24,6,
			4,16,24,16,4,
			1, 4, 6, 4,1 );

void main(void)
{
	vec3 acc = vec3(0,0,0);

	/* Side = 1 ger 3x3 filter */
	/* 	= 2 ger 5x5 filter... */
	const int side = 2;
	int area = 256;
	for(int x = -side; x <= side; x++){
		for(int y = -side; y <= side; y++){
			acc += gauss_coeff[(y+side)*(side*2+1)+(x+side)]*vec3(textureOffset(texUnit, outTexCoord, ivec2(x, y)));
		}
	}
	acc = acc / area;

	out_Color = vec4(acc, 1.0);
}
