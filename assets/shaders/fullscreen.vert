#version 330

out vec2 tex_coord;
uniform bool  shake;
uniform float time;
void main(){

    // These positions and their texture coordinates define a fullscreen triangle 
    vec2 positions[] = vec2[](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );

    // The texture coordinates are picked such that the texture coordinates
    // at the bottom left corner of the screen are (0,0) and
    // at the top right corner of the screen are (1,1)
    vec2 tex_coords[] = vec2[](
        vec2( 0.0,  0.0),
        vec2( 2.0,  0.0),
        vec2( 0.0,  2.0)
    );

    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    tex_coord = tex_coords[gl_VertexID];
    if (shake)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;        
        gl_Position.y += cos(time * 15) * strength;        
    }
}