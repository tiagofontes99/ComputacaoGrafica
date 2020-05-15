//
//  main.cpp
//  OpenGLNewProject
//
//

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "/home/tiago/Documents/ComputacaoGrafica/libs/stb_image.h"

#include "/home/tiago/Documents/ComputacaoGrafica/glm-0.9.9.7/glm/glm/glm.hpp"
#include "/home/tiago/Documents/ComputacaoGrafica/glm-0.9.9.7/glm/glm/gtc/type_ptr.hpp"
#include "/home/tiago/Documents/ComputacaoGrafica/glm-0.9.9.7/glm/glm/gtc/matrix_transform.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "/home/tiago/Documents/ComputacaoGrafica/libs/Shader.hh"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);


glm::vec4 toBezier(float delta, int i, glm::vec4 P0, glm::vec4 P1, glm::vec4 P2, glm::vec4 P3)
{
    float t=delta*float(i);
    //todo
    // Point = (1−t)^3𝑃0+ 3(1−t)^2 t𝑃1 + 3(1−t)t^2𝑃3 + t^3𝑃4 
    return (1.0f -t)*(1.0f -t)*(1.0f -t)*P0 + 3*(1.0f -t)*(1.0f -t) * t *P1+3*(1.0f-t)*t*t*P2+t*t*t*P3;
}



int main(int argc, const char * argv[]) {
    //************************** CONTEXT **********************************
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);  // anti-aliasing factor
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);  //resizable window = false
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy;
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // use modern openGL
    
    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "hello", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Initialize GLEW
    glewInit();
    //************************************************************
    
    
    
    int nSegs = 100;
    float delta = 1.0/nSegs;
    std::vector<float> vertices2;
    glm::vec4 p0 = glm::vec4(-1.0f, -0.5f,  0.0f, 0.0f);
    glm::vec4 p1 = glm::vec4(-0.5f, 1.0f,  0.0f, 0.0f);
    glm::vec4 p2 = glm::vec4(-1.0f, -2.0f,  0.0f, 0.0f);
    glm::vec4 p3 = glm::vec4(1.0f, -3.0f,  0.0f, 0.0f);
    for(int i=0; i<nSegs+1; i++){
        glm::vec4 var= toBezier(delta ,i ,p0, p1,p2,p3);
        vertices2.push_back(var.x);
        vertices2.push_back(var.y);
        vertices2.push_back(var.z);
    }
    
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2[0]) * vertices2.size(), &vertices2[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Shader ourShader("/home/tiago/Documents/ComputacaoGrafica/libs/shader.vs", "/home/tiago/Documents/ComputacaoGrafica/libs/shader.fs");
    
    while (!glfwWindowShouldClose(window)){
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // activate shader
        ourShader.use();
        glBindVertexArray(VAO);
        
        //glDrawArrays(GL_LINE_STRIP, 0, 3);
        glDrawArrays(GL_LINE_STRIP, 0, vertices2.size()/3);

        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    std::cout << "Hello, World!\n";
    glfwTerminate();
    return 0;
}


// ------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
