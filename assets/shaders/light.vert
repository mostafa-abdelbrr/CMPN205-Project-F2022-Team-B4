#version 330 core

// TODO:Add support for lighting in the shaders.

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

uniform mat4 transform;
uniform mat4 transform_IT;
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

    vs_out.world = (transform * vec4(position, 1.0f)).xyz;
    vs_out.view = eye - vs_out.world;
    vs_out.normal = normalize((transform_IT * vec4(normal, 0.0f)).xyz);
    gl_Position = VP * vec4(vs_out.world, 1.0);
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;

}