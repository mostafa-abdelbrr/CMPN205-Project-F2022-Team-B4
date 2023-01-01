#version 330 core
in vec2 tex_coord;
out vec4 frag_color;

uniform sampler2D tex;

void main()
{
    float red = 1.0-texture(tex,tex_coord).r;
    float blue = 1.0-texture(tex,tex_coord).b;
    float green = 1.0-texture(tex,tex_coord).g;

	frag_color = vec4(red, green, blue, 1.0);
}