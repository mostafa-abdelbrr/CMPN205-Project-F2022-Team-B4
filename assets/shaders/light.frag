#version 330 core

// TODO:Add support for lighting in the shaders.

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} fs_in;

// defining the different kinds of light 
#define TYPE_DIRECTIONAL 0
#define TYPE_POINT 1
#define TYPE_SPOT 2

struct Light {
    int type;
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 position, direction; //we get these from the owning entity
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;
    float inner_angle, outer_angle;
};

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
    };

uniform Material material;

// defining a maximum to the array of lights passed to the fragment shader
#define MAX_LIGHT_COUNT 8
// a uniform holding the array of lights since we are using the single pass forward lighting approach
uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;

out vec4 frag_color;

void main() {

    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);
    // finding the minimum between the 2 in case of a bug in the light count passed
    int count = min(light_count, MAX_LIGHT_COUNT);
    vec3 accumulated_light = vec3(0.0);
    // looping over all the lights passed in the array
    for(int index = 0; index < count; index++){

        Light light = lights[index];
        vec3 light_direction;
        float attenuation = 1;
        // if the light is directional there is no attenuation and light only has one direction
        if(light.type == TYPE_DIRECTIONAL)
        {
            light_direction = light.direction;
        }

        else {

            light_direction = fs_in.world - light.position;
            float distance = length(light_direction);
            light_direction /= distance;

            attenuation *= 1.0f / (light.attenuation_constant +light.attenuation_linear * distance + light.attenuation_quadratic * distance * distance);

            if(light.type == TYPE_SPOT){

                float angle = acos(dot(light.direction, light_direction));
                attenuation *= smoothstep(light.outer_angle, light.inner_angle, angle);

            }

        }

        vec3 reflected = reflect(light_direction, normal);

        float lambert = max(0.0f, dot(normal, -light_direction));
        float phong = pow(max(0.0f, dot(view, reflected)), material.shininess);

        vec3 diffuse = material.diffuse * light.diffuse * lambert;
        vec3 specular = material.specular * light.specular * phong;
        vec3 ambient = material.ambient * light.ambient;
        accumulated_light += (diffuse + specular) * attenuation + ambient;
    }

    frag_color = fs_in.color * vec4(accumulated_light, 1.0f);

}