#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    // getting the size of the viewport using the gl_FragCoord.xy variable
    // and dividing it into size+size square pixels using the mod function
    // gl_FragCoord.xy is the position of a pixel in the screen
    vec2 pos = mod(gl_FragCoord.xy,vec2(size+size));
    // we compare the pixel coordinates with the size of the squares to get the pixel color
    if ((pos.x > size)&&(pos.y > size) || (pos.x <size)&&(pos.y < size)) //odd squares with color[0]
    {
        frag_color = vec4(colors[0], 1.0);
    }

    if ((pos.x < size)&&(pos.y > size) || (pos.x > size)&&(pos.y < size)) //even squares with color[0]
    {
        frag_color = vec4(colors[1], 1.0);
    }

}