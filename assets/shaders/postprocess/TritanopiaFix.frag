#version 330 core
in vec2 tex_coord;
out vec4 frag_color;

const mat3 RGBtoOpponentMat = mat3(0.2814, -0.0971, -0.0930, 0.6938, 0.1458,-0.2529, 0.0638, -0.0250, 0.4665);
const mat3 OpponentToRGBMat = mat3(1.1677, 0.9014, 0.7214, -6.4315, 2.5970, 0.1257, -0.5044, 0.0159, 2.0517);

uniform sampler2D tex;

void main()
{
    float red = texture(tex,tex_coord).r;
    float blue = texture(tex,tex_coord).b;
    float green = texture(tex,tex_coord).g;
    vec3 opponentColor = RGBtoOpponentMat * vec3(red, green, blue);
	opponentColor.x -= ((3.0 * opponentColor.z) - opponentColor.y) * 0.25;
	vec3 rgbColor = OpponentToRGBMat * opponentColor;
    vec3 blindVisionR = vec3(0.97,  0.11, -0.08);
    vec3 blindVisionG = vec3(0.02,  0.82,  0.16);
	vec3 blindVisionB = vec3(-0.06,  0.88,  0.18);	
	frag_color = vec4(dot(rgbColor,blindVisionR), dot(rgbColor,blindVisionG), dot(rgbColor,blindVisionB), 1.0);
}