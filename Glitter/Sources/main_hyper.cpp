
#define EDGE_LENGTH     0.555892971716750006194729394337628037
#define COORD1          0.543761052313176840300457115517929196
#define COORD2          0.081672865724627272121516341485403245

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

// Local Headers

#include "glitter.hpp"
#include "shader.h"
#include "/Users/alexxx/HyperbolicTessellations/Glitter/Headers/hyperedge.hpp"

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
    
    Shader Shader0("/Users/alexxx/Glitter/Glitter/Shaders/vertex_shader0.vs", "/Users/alexxx/Glitter/Glitter/Shaders/fragment_shader0.frag");
    
    
    
    
    
    
//    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
//    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    

    //Prepare the edge!!!
    hyperedge hyperedge0(EDGE_LENGTH, 0.05, 6);
    
    std::cout << "hyperedge: " <<hyperedge0.get_vertices().size()/3 <<" "<<hyperedge0.get_indices().size()/3<< std::endl;

    GLuint hyperedge0_VBO, hyperedge0_normals, hyperedge0_VAO, hyperedge0_EBO;
    glGenVertexArrays(1, &hyperedge0_VAO);
    glGenBuffers(1, &hyperedge0_VBO);
    glGenBuffers(1, &hyperedge0_normals);
    glGenBuffers(1, &hyperedge0_EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(hyperedge0_VAO);
    
    // Position attribute
    glBindBuffer(GL_ARRAY_BUFFER, hyperedge0_VBO);
    glBufferData(GL_ARRAY_BUFFER, hyperedge0.get_vertices().size()*sizeof(GLfloat), &hyperedge0.get_vertices()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glBindBuffer(GL_ARRAY_BUFFER, hyperedge0_normals);
    glBufferData(GL_ARRAY_BUFFER, hyperedge0.get_normals().size()*sizeof(GLfloat), &hyperedge0.get_normals()[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    
    
    
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hyperedge0_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, hyperedge0.get_indices().size()*sizeof(GLuint), &hyperedge0.get_indices()[0], GL_STATIC_DRAW);
    //    std::cout << "sizeof(vertices): " <<hyperedge0.get_vertices_length()*sizeof(GLfloat)*3<< std::endl;
    //    std::cout << "sizeof(indices): " <<hyperedge0.get_indices_length()*sizeof(GLuint)*3<< std::endl;
    
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    
    // Uncommenting this call will result in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Game loop
    Shader0.Use();
    while (!glfwWindowShouldClose(mWindow))
    {
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
        
        glm::vec3 light_position(10.0f, 0.0f, 10.0f);
        GLint light_position_location = glGetUniformLocation(Shader0.Program, "light_position");
        glUniform3f(light_position_location, light_position.x, light_position.y, light_position.z);
        
        glm::vec3 light_color(1.0f, 1.0f, 1.0f);
        GLint light_color_location = glGetUniformLocation(Shader0.Program, "light_color");
        glUniform3f(light_color_location, light_color.x, light_color.y, light_color.z);
        
        glm::mat4 proj = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view;
        glm::mat4 model;
        
        model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        
//        glm::mat4 view2 = glm::translate(view, glm::vec3(5.0f, 0.0f,  0.0f));
        glm::mat4 view3 = glm::rotate(view, 0.0f, glm::vec3(0.0f, 1.0f,  0.0f));
        glm::mat4 view4 = glm::translate(view, glm::vec3(0.0f, 0.0f,  -15.0f));
        view = view4*view3;
        
        glm::vec3 color(0.4f, 0.0f, 0.8f);
        
        GLint modelLoc = glGetUniformLocation(Shader0.Program, "model");
        GLint projLoc = glGetUniformLocation(Shader0.Program, "projection");
        GLint viewLoc = glGetUniformLocation(Shader0.Program, "view");
        GLint colorLoc = glGetUniformLocation(Shader0.Program, "color");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniform3fv(colorLoc,1, glm::value_ptr(color));

        glDrawElements(GL_TRIANGLES, hyperedge0.get_indices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        
        
        // Swap the screen buffers
        glfwSwapBuffers(mWindow);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &hyperedge0_VAO);
    glDeleteBuffers(1, &hyperedge0_VBO);
    glDeleteBuffers(1, &hyperedge0_EBO);
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
