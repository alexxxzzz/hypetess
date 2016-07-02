// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    
    GLfloat verticesBack[] = {
        //locations             colors
        -0.5f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f,   // Top Left Back
        -0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f,  // Bottom Left Back
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, 0.0f,  // Bottom Right Back
        0.5f,  0.5f, 0.5f,      1.0f, 0.0f, 0.0f,  // Top Right Back
        
    };
    GLfloat verticesFront[] = {
        0.5f,  0.5f, -0.5f,      1.0f, 1.0f, 0.0f,  // Top Right Front
        0.5f, -0.5f, -0.5f,      1.0f, 1.0f, 0.0f,  // Bottom Right Front
        -0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 0.0f,  // Bottom Left Front
        -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f   // Top Left Front
        
    };
    GLfloat verticesLeft[] = {
        -0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 0.0f,  // Bottom Left Back
        -0.5f,  0.5f, 0.5f,      0.0f, 1.0f, 0.0f,  // Top Left Back
        -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   // Top Left Front
        -0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,  // Bottom Left Front
    };
    GLfloat verticesRight[] = {
        0.5f,  0.5f, 0.5f,      0.0f, 1.0f, 1.0f,  // Top Right Back
        0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 1.0f,  // Bottom Right Back
        0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 1.0f,   // Bottom Right Front
        0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 1.0f,  // Top Right Front
    };
    GLfloat verticesTop[] = {
        0.5f,  0.5f, 0.5f,      0.0f, 0.0f, 1.0f,  // Top Right Back
        -0.5f,  0.5f, 0.5f,     0.0f, 0.0f, 1.0f,  // Top Left Back
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,  // Top Left Front
        0.5f,  0.5f, -0.5f,     0.0f, 0.0f, 1.0f,  // Top Right Front
        
    };
    GLfloat verticesBottom[] = {
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f, 1.0f,  // Bottom Right Back
        -0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 1.0f,  // Bottom Left Back
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 1.0f,  // Bottom Left Front
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f,  // Bottom Right Front
        
    };
    
    
    GLfloat* vertex_sets[] = {
        verticesBack,verticesFront,
        verticesLeft,verticesRight,
        verticesTop,verticesBottom,
    };
    
    GLuint indices[] = {
        0, 1, 2,  // First Triangle
        2, 3, 0   // Second Triangle
    };
    const int N_sides=6;
    
    GLuint VBOs[N_sides], VAOs[N_sides], EBOs[N_sides];
    glGenVertexArrays(N_sides, VAOs);
    glGenBuffers(N_sides, VBOs);
    glGenBuffers(N_sides, EBOs);
    for(int k=0;k<N_sides;k++){
        // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
        glBindVertexArray(VAOs[k]);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[k]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBack), vertex_sets[k], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[k]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        
    }
    
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*for(int k=0;k<N_sides;k++){
         glBindVertexArray(VAOs[k]);
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
         glBindVertexArray(0);
         }*/

        
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        
        /*        Shader1.Use();
         GLint modelLoc = glGetUniformLocation(Shader1.Program, "model");
         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
         GLint projLoc = glGetUniformLocation(Shader1.Program, "projection");
         glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
         GLint viewLoc = glGetUniformLocation(Shader1.Program, "view");
         glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
         */
        //   glBindVertexArray(lines0_VAO);
        //   glDrawElements(GL_LINES, tentacle0.lines.size(), GL_UNSIGNED_INT, 0);
        //   glBindVertexArray(0);
        
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
