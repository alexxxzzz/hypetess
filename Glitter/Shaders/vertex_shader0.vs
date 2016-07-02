#version 400 core
#define M_PI        3.14159265358979323846264338327950288   /* pi */

layout (location = 0) in vec3 position_vs;
layout (location = 1) in vec3 normal_vs;
layout (location = 2) in vec3 color_vs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec3 color_fs;
out vec3 normal_fs;
out vec3 fragment_position_fs;

void main()
{
    
    gl_Position = projection * view * model * vec4(position_vs, 1.0f);
    color_fs = color;
    fragment_position_fs = vec3(view * model * vec4(position_vs, 1.0f));
    normal_fs  = mat3(transpose(inverse(view))) * mat3(transpose(inverse(model))) * normal_vs;
    
}