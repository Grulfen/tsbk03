#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections

uniform sampler2D texUnit;
uniform sampler2D texUnit2;
uniform float time;

out vec4 out_Color;
vec3 normal;

void main(void)
{
    vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates
    vec2 tmpTexCoord;

    mat3 Mvt = transpose(mat3(Ps,Pt,out_Normal));

    // Calculate gradients here
    float offset = 1.0 / 256.0; // texture size, same in both directions

    tmpTexCoord = vec2(outTexCoord.s+offset, outTexCoord.t);
    float dbs = texture(texUnit, tmpTexCoord);
    tmpTexCoord.s -= 2*offset; 
    dbs -= texture(texUnit, tmpTexCoord);
    dbs *= 2;

    tmpTexCoord = vec2(outTexCoord.s, outTexCoord.t+offset);
    float dbt = texture(texUnit, tmpTexCoord);
    tmpTexCoord.t -= 2*offset; 
    dbt -= texture(texUnit, tmpTexCoord);
    dbt *= 2;

    normal = vec3(dbs, dbt, 1);
    normal = normalize(normal);

    // Simplified lighting calculation.
    // A full solution would include material, ambient, specular, light sources, multiply by texture.
    out_Color = vec4( dot(normal, Mvt*light));
    /* out_Color = vec4( dot(normal, Mvt*light)) * texture(texUnit2, outTexCoord); */
}
