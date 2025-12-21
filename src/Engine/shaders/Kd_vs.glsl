#version 420

layout(location=0) in vec3 a_vertex_position;
layout(location=3) in vec2 a_vertex_texture;

out vec2 vertex_texture;

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

void main() {
    vertex_texture = a_vertex_texture;
    gl_Position = PVM * vec4(a_vertex_position, 1.0);
}