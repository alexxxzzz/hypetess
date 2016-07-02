// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

// Local Headers

#include "glitter.hpp"
#include "shader.h"
#include "Utentacle.h"
#include "Usphere.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;



// The MAIN function, from here we start the application and run the game loop
int main()
{
    srand (time(NULL));
    std::cout << "Starting GLFW context, OpenGL 4.0" << std::endl;
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
    
    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }
    
    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    
    // Define the viewport dimensions
    //glViewport(0, 0, WIDTH, HEIGHT);
    
    // Build and compile our shader program
    // Vertex shader
    
    Shader Shader0("/Users/alexxx/Glitter/Glitter/Shaders/tentacle_vertex_shader.vs", "/Users/alexxx/Glitter/Glitter/Shaders/fragment_shader0.frag");
    
    
    
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    

    //Prepare the tentacle!!!
    unit_tentacle tentacle0(M_PI*2*3,10.0f,0.05, 8, 0.0f);
    //tentacle0.set_color_solid(0.4f, 0.0f, 0.8f);
    tentacle0.set_color_gradient(glm::vec3(0.4f,0.0f,0.8f),glm::vec3(0.4f,1.0f,0.8f));
    
    std::cout << "tentacle:" <<tentacle0.get_vertices().size()/3 <<" "<<tentacle0.get_indices().size()/3<< std::endl;

    GLuint tentacle0_VBO, tentacle0_normals, tentacle0_colors, tentacle0_VAO, tentacle0_EBO;
    glGenVertexArrays(1, &tentacle0_VAO);
    glGenBuffers(1, &tentacle0_VBO);
    glGenBuffers(1, &tentacle0_colors);
    glGenBuffers(1, &tentacle0_normals);
    glGenBuffers(1, &tentacle0_EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(tentacle0_VAO);
    
    // Position attribute
    glBindBuffer(GL_ARRAY_BUFFER, tentacle0_VBO);
    glBufferData(GL_ARRAY_BUFFER, tentacle0.get_vertices().size()*sizeof(GLfloat), &tentacle0.get_vertices()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glBindBuffer(GL_ARRAY_BUFFER, tentacle0_normals);
    glBufferData(GL_ARRAY_BUFFER, tentacle0.get_normals().size()*sizeof(GLfloat), &tentacle0.get_normals()[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    
    // Color attribute
    glBindBuffer(GL_ARRAY_BUFFER, tentacle0_colors);
    glBufferData(GL_ARRAY_BUFFER, tentacle0.get_vertices().size()*sizeof(GLfloat), &tentacle0.get_colors()[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tentacle0_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tentacle0.get_indices().size()*sizeof(GLuint), &tentacle0.get_indices()[0], GL_STATIC_DRAW);
    //    std::cout << "sizeof(vertices): " <<tentacle0.get_vertices_length()*sizeof(GLfloat)*3<< std::endl;
    //    std::cout << "sizeof(indices): " <<tentacle0.get_indices_length()*sizeof(GLuint)*3<< std::endl;
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    
    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Game loop
    float angle = 0.0;
    GLfloat bend_angle  = 0.0;
    GLfloat max_angle = tentacle0.get_theta_max();
    GLfloat delta_bend_angle = 0.05;
    GLfloat delta_dist = 0.001f;
    GLfloat dist = 0.0f;
    int j_start = 0;
    int j_end = 0;
    int green_dir_start = 1;
    GLfloat green_start = 0.0f;
    GLfloat N = 8.0f;
    GLfloat M = 45.0f;
    GLfloat R_tunnel = tentacle0.get_length()+2.0;
    GLfloat curvature = 50.0f;
    GLfloat tentacle_distance = 2.0f;
    GLfloat phi = tentacle_distance/curvature;
    phi = 2*M_PI / glm::floor(2*M_PI / phi);
    GLfloat delta_green = 8.0f / (2*M_PI / phi);

    Shader0.Use();
    while (!glfwWindowShouldClose(mWindow))
    {
        angle = angle+0.0f;
        
        bend_angle = bend_angle + delta_bend_angle;
        if(glm::abs(bend_angle)>max_angle){
            delta_bend_angle = delta_bend_angle * -1.0f;
            bend_angle = bend_angle + delta_bend_angle;
        }
        

        
        
        
        //GLfloat bend_angle
        
        //angle = 1.0;
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Wire frame mode
        //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        
        GLint length_spiralLoc = glGetUniformLocation(Shader0.Program, "length_spiral");
        glUniform1f(length_spiralLoc, tentacle0.get_length());
        
        GLint b_spiralLoc = glGetUniformLocation(Shader0.Program, "b_spiral");
        glUniform1f(b_spiralLoc, tentacle0.get_spiral_growth_rate());
        
        GLint theta_endLoc = glGetUniformLocation(Shader0.Program, "theta_end");
        glUniform1f(theta_endLoc, bend_angle);
        
        glm::vec3 light_position(10.0f, 0.0f, 10.0f);
        GLint light_position_location = glGetUniformLocation(Shader0.Program, "light_position");
        glUniform3f(light_position_location, light_position.x, light_position.y, light_position.z);
        
        glm::vec3 light_color(1.0f, 1.0f, 1.0f);
        GLint light_color_location = glGetUniformLocation(Shader0.Program, "light_color");
        glUniform3f(light_color_location, light_color.x, light_color.y, light_color.z);
        
        
        glm::mat4 proj = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view;
        glm::mat4 view2;
        glm::mat4 view3;
        glm::mat4 model;
        glm::mat4 model2;
        
        
        GLint modelLoc = glGetUniformLocation(Shader0.Program, "model");
        GLint projLoc = glGetUniformLocation(Shader0.Program, "projection");
        GLint viewLoc = glGetUniformLocation(Shader0.Program, "view");
        GLint colorLoc = glGetUniformLocation(Shader0.Program, "color_ext");
        
        glBindVertexArray(tentacle0_VAO);
        
        dist = dist + delta_dist;
        if(dist > (j_start+1)*phi){
            j_start++;
            green_start = green_start + green_dir_start*delta_green;
            if(green_start>1.0f){
                green_dir_start = -1;
                green_start = green_start + green_dir_start*2.0f*delta_green;
            }
            if(green_start<0.0f){
                green_dir_start = 1;
                green_start = green_start + green_dir_start*2.0f*delta_green;
            }
        }
        j_end = j_start + M;
        
        GLfloat green = green_start;
        int green_dir = green_dir_start;
        
        for(int j=j_start;j<j_end;j++){
            green = green + green_dir*delta_green;
            if(green>1.0f){
                green_dir = -1;
                green = green + green_dir*2.0f*delta_green;
            }
            if(green<0.0f){
                green_dir = 1;
                green = green + green_dir*2.0f*delta_green;
            }
            glm::vec3 color(0.4f, green, 0.8f);
            glUniform3fv(colorLoc,1, glm::value_ptr(color));
            
            //move the ring of tentacles to the curvature radius
            view3 = glm::translate(view, glm::vec3(curvature, 0.0f, -0.0f));
            //and rotate them by the correct angle
            view3 = glm::rotate(view, phi * j, glm::vec3(0.0f, 1.0f, 0.0f)) * view3;
            //rotate everything by dist angle
            view3 = glm::rotate(view, -dist, glm::vec3(0.0f, 1.0f, 0.0f)) * view3;
            //move them back
            view3 = glm::translate(view, glm::vec3(-curvature, 0.0f, -0.0f)) * view3;
            
            for(int k=0;k<N;k++){
                GLfloat theta = M_PI/2.0;
                
                //rotate the tentacle on itself so it will point to the centre
                model2 = glm::rotate(model, theta, glm::vec3(0.0f, 0.0f, 1.0f));
                
                theta = M_PI * (k * 2.0f + j%2) / N;
                
                //move the tentacle to the tunnel radius location
                view2 = glm::translate(view, glm::vec3(R_tunnel, 0.0f, -0.0f));
                //and rotate around z-axis to put it in its location
                view2 = glm::rotate(view, theta, glm::vec3(0.0f, 0.0f, 1.0f)) * view2;
                
                view2 = view3 * view2;
                
                
                
                view2 = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f)) * view2;
                //view3 = glm::rotate(view3, 1.57f, glm::vec3(1.0f, 0.0f, 0.0f));
                //view3 = glm::translate(view3, glm::vec3(-curvature*0, 0.0f,  -curvature));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view2));
                glDrawElements(GL_TRIANGLES, tentacle0.get_indices().size(), GL_UNSIGNED_INT, 0);
                
            }
        }
        glBindVertexArray(0);
        
        
        
        // Swap the screen buffers
        glfwSwapBuffers(mWindow);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &tentacle0_VAO);
    glDeleteBuffers(1, &tentacle0_VBO);
    glDeleteBuffers(1, &tentacle0_EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
