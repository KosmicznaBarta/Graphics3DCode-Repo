#version 420

const int MAX_POINT_LIGHTS = 16;
const float INV_PI = 1.0/3.14159265;

in vec2 vertex_texture;
in vec3 vertex_normal_vs;
in vec3 vertex_position_vs;
uniform sampler2D map_Kd;

struct PointLight {
       vec3 position;
       float radius;
       vec3 color;
       float intensity;
    };

layout(location=0) out vec4 vFragColor;

layout(std140, binding=0) uniform KdMaterial {
       vec4 Ka;
       vec4 Kd;
       vec4 KsNs;
       bool use_vertex_color; 
       bool use_map_Kd;
    };
   
layout (std140, binding = 2) uniform Lights {
       vec3 ambient;
       int n_lights;
       PointLight lights[MAX_POINT_LIGHTS];
    };

vec3 srgb_gamma_correction(vec3 color) {
    color = clamp(color, 0.0, 1.0);
    color = mix(
        color * 12.92,
        (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055,
        step(0.0031308, color)
    );
    return color;
}

void main() {
    vec4 color = Kd;
    vec3 normal = normalize(vertex_normal_vs);
    vec3 result = vec3(0.0);
    vec3 view = normalize(-vertex_position_vs);
    vec3 Ks = KsNs.xyz;
    float Ns = KsNs.w;

    if (!gl_FrontFacing) {
        normal = -normal;
    }
    else {
        result += Ka.rgb * ambient;
    }

    if (use_map_Kd) {
        color *= texture(map_Kd, vertex_texture);
    }

    vFragColor.a = color.a;

    for (int i = 0; i < n_lights; ++i) {
        vec3 light = lights[i].position - vertex_position_vs;
        float light_distance = length(light);
        light = normalize(light);
        vec3 halfvec = normalize(light + view);

        float diffuse = max(dot(normal, light), 0.0);

        float r = max(lights[i].radius, light_distance);
        float attenuation = 1.0 / (r * r);
        float spec_component = pow(max(dot(normal,halfvec),0.0),Ns);
        vec3 specular = ((Ns+8.0)/8)*INV_PI*spec_component*Ks;


        result += ((INV_PI * color.rgb)+specular) * lights[i].color * lights[i].intensity * diffuse * attenuation;
    }
    
    vFragColor = vec4(srgb_gamma_correction(result), color.a);
}