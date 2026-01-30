#version 420

layout(location=0) in vec3 a_vertex_position;
layout(location=3) in vec2 a_vertex_texture;
layout(location=1) in vec3 a_vertex_normal;

out vec2 vertex_texture;
out vec3 vertex_normal_vs;
out vec3 vertex_position_vs;

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
    mat4 VM;
    mat3 VM_Normal;
};

void main() {
    vertex_texture = a_vertex_texture;
    vertex_normal_vs = normalize(VM_Normal * a_vertex_normal);
    vec4 a_vertex_position_vs = VM * vec4(a_vertex_position, 1.0);
    vertex_position_vs = a_vertex_position_vs.xyz/a_vertex_position_vs.w;
    gl_Position = PVM * vec4(a_vertex_position, 1.0);
}