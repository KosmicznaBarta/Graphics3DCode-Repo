#version 420

in vec4 vertex_color;
layout(location=0) out vec4 vFragColor;

layout(std140, binding = 0) uniform Mixer {
    float strength;
    vec3  mix_color; 
   };

void main() {
    //oddzielamy rgb od alfa
    vFragColor.rgb = mix(vertex_color.rgb, mix_color, strength);
    vFragColor.a = vertex_color.a;
}
