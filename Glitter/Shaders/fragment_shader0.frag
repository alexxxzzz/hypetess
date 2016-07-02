#version 400 core

in vec3 normal_fs;
in vec3 fragment_position_fs;

out vec4 color;

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 color_ext;

void main()
{
    float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * color_ext;
    vec3 norm = normalize(normal_fs);
    vec3 light_direction = normalize(light_position - fragment_position_fs);
    vec3 diffuse = max(dot(norm, light_direction), 0.0) * light_color;
    
    float specular_strength = 0.5f;
    vec3 view_direction = normalize(-fragment_position_fs);
    vec3 reflect_direction = reflect(-light_direction, norm);
    vec3 specular = specular_strength * pow(max(dot(view_direction, reflect_direction), 0.0), 32) * light_color;
    
    
    vec3 result = (diffuse + ambient + specular) * color_ext;
//    vec3 result = color_fs;
    color = vec4(result, 1.0f);
}

