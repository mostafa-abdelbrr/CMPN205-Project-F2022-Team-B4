#version 330 core

// TODO:Add support for lighting in the shaders.

in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 world;
    vec3 view;
    vec3 normal;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;

struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emission;
};

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light {
   int type;
    vec3 color;
    vec3 position, direction; //we get these from the owning entity
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;
    float inner_angle, outer_angle;
};


// Defining a maximum to the array of lights passed to the fragment shader
#define MAX_LIGHT_COUNT 8
// A uniform holding the array of lights since we are using the single pass forward lighting approach
uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;
uniform Material material;

void main(){

    vec3 normal = normalize(fs_in.normal);//normal on point
    vec3 view = normalize(fs_in.view);//from camera to POINT

    //we read from all texures simultanously as we created different texure units
    vec3 material_albedo = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 material_specular = texture(material.specular, fs_in.tex_coord).rgb;
    float roughness = texture(material.roughness, fs_in.tex_coord).r;//take only r channel as it is grayscale
    float shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;
    vec3 material_emission = texture(material.emission, fs_in.tex_coord).rgb;
    float material_ao = texture(material.ambient_occlusion, fs_in.tex_coord).r;
    
    // Initially the accumulated light will hold the ambient light and the emissive light (light coming out of the object).
    vec3 accumulated_light = vec3(0.0);
    accumulated_light =material_emission + material_albedo * material_ao;
    
    // Make sure that the actual light count never exceeds the maximum light count.
    int count = min(light_count, MAX_LIGHT_COUNT);

     // Now we will loop over all the lights.
    for(int i = 0; i < count; i++){
        Light light = lights[i];

        vec3 light_vec = - light.direction;//as lambart assume light direction from point to light
        float attenuation = 1;

        // the following calculations, like attenuation and position are only done for point and spot lights
        if(light.type != DIRECTIONAL){

            light_vec = normalize(light.position - fs_in.world);

            // attenuation is only computed if the light is not directional
            float d = distance(light.position, fs_in.world);
            attenuation =attenuation*( 1 / (light.attenuation_constant +(light.attenuation_linear * d) + light.attenuation_quadratic * d * d));
            
            if(light.type == SPOT){
                // If it is a spot light, compute the angle attenuation.
                // outside the outer cone light is 0 inside the inner cone light is at full ingtensity and between
                // the 2 angles light is gradually decreasing in intensity
                float angle = acos(-dot(light.direction, light_vec));
                attenuation *= smoothstep(light.outer_angle, light.inner_angle, angle);
            }
        }
         vec3 reflected = reflect(-light_vec, normal);
        // we prepare the lambert and the phong equations used in the 2 light componants
        float lambert = max(0.0f, dot(normal, light_vec));
        float phong = pow(max(0.0f, dot(view, reflected)),shininess);

        // Now we compute the 2 components of the light separately.
        vec3 diffuse = material_albedo * light.color * lambert;
        vec3 specular = material_specular * light.color * phong;

        // Then we accumulate the light components additively.
        accumulated_light+= (diffuse + specular) * attenuation; // + ambient;
    }
    frag_color = fs_in.color * vec4(accumulated_light, 1.0f);
}