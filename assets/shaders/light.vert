#version 330 core

// TODO:Add support for lighting in the shaders.

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

// this is the normal transformation matrix that is used in the textured material
uniform mat4 transform;
// thi is the model matrix to transform from the object space to the world space
// used because in the case of lighting we need to access this coordinate system before the multiplication with VP
uniform mat4 model;
// the inverse transform of the model matrix used to get the normal
uniform mat4 model_IT;
uniform mat4 VP;
uniform vec3 eye;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} vs_out;

void main() {
    // this vertex shader is different because we need to seperate the world space because this is where the effect 
    // of the lighting takes place
    vec3 world = (model * vec4(position, 1.0)).xyz;
    vs_out.world = world;
    gl_Position = VP * vec4(world, 1.0);

    vs_out.normal =normalize( (model * vec4(normal, 0.0)).xyz);
    vs_out.view = eye - world;
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;

}