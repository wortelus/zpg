#version 330

in vec4 ex_world_position;
in vec3 ex_world_normal;
in vec3 ex_view_direction;

const int MAX_LIGHTS = 10;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 object_color;
uniform Material material;
uniform PointLight lights[MAX_LIGHTS];
uniform int num_lights;

out vec4 out_color;

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 light_direction_n = normalize(light.position - fragPos);
    float diff = max(dot(normal, light_direction_n), 0.0);

    vec3 reflect_direction = reflect(-light_direction_n, normal);
    float spec = pow(max(dot(viewDir, reflect_direction), 0.0), material.shininess);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 ambient = material.ambient;
    vec3 diffuse = material.diffuse * diff;

    // TODO: specular color based on light color
    vec3 specular = material.specular * spec;

    ambient *= light.intensity * attenuation;
    diffuse *= light.intensity * attenuation;
    specular *= light.intensity * attenuation;

    return (ambient + diffuse + specular) * object_color;
}

void main(void) {
    vec3 view_direction_norm = normalize(ex_view_direction);
    vec3 world_normal_norm = normalize(ex_world_normal);

    vec3 result = vec3(0.0);
    for(int i = 0; i < num_lights; ++i) {
        result += calcPointLight(lights[i], world_normal_norm, ex_world_position.xyz, view_direction_norm);
    }

    out_color = vec4(result, 1.0);
}