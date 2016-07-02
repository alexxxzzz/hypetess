#version 400 core
#define M_PI        3.14159265358979323846264338327950288   /* pi */

layout (location = 0) in vec3 position_vs;
layout (location = 1) in vec3 normal_vs;
layout (location = 2) in vec3 color_vs;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float length_spiral;
uniform float b_spiral;
uniform float theta_end;


out vec3 color_fs;
out vec3 normal_fs;
out vec3 fragment_position_fs;

void main()
{
    float vertex_x = position_vs.x;
    float vertex_y = position_vs.y;
    float normal_x = normal_vs.x;
    float normal_y = normal_vs.y;

    if(abs(theta_end)>0.0001){
        float phi = abs(atan(-1.0f/b_spiral));
        float R_spiral = length_spiral*cos(phi)/(1.0f-exp(b_spiral*abs(theta_end)));
        float theta_scale = abs(theta_end/length_spiral);
        float cos_sign = sign(theta_end);
        float corr_angle = (phi - M_PI/2.0f)*cos_sign;
        float theta = theta_scale * position_vs.y;
        float normal_angle = (M_PI - phi + theta - M_PI/2.0f)*cos_sign;
    
        float vertex_x_rotated = position_vs.x * cos(normal_angle);
        float vertex_y_rotated = position_vs.x * sin(normal_angle);
    
        float vertex_x0 = (R_spiral*exp(b_spiral * theta) * cos(theta)  - R_spiral) * cos_sign + vertex_x_rotated;
        float vertex_y0 =  R_spiral*exp(b_spiral * theta) * sin(theta)+ vertex_y_rotated;
        float normal_x0 = normal_vs.x * cos(normal_angle) - normal_vs.y * sin(normal_angle);
        float normal_y0 = normal_vs.x * sin(normal_angle) + normal_vs.y * cos(normal_angle);
    
        vertex_x = vertex_x0 * cos(corr_angle) - vertex_y0 * sin(corr_angle);
        vertex_y = vertex_x0 * sin(corr_angle) + vertex_y0 * cos(corr_angle);
        normal_x = normal_x0 * cos(corr_angle) - normal_y0 * sin(corr_angle);
        normal_y = normal_x0 * sin(corr_angle) + normal_y0 * cos(corr_angle);
    }
    
    
    vec3 position_vertex = vec3(vertex_x, vertex_y, position_vs.z);
    
    gl_Position = projection * view * model * vec4(position_vertex, 1.0f);
    //  gl_Position = model * vec4(position, 1.0f);
    //normal_fs = vec3(model * vec4(normal_vs, 1.0f));
    fragment_position_fs = vec3(view * model * vec4(position_vertex, 1.0f));
    normal_fs  = mat3(transpose(inverse(view))) * mat3(transpose(inverse(model))) * vec3(normal_x, normal_y, normal_vs.z);
    color_fs = color_vs;
}