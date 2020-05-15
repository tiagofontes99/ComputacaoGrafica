// http://www.opengl-tutorial.org/
// https://learnopengl.com/Getting-started/Hello-Triangle
// https://open.gl/drawing

#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main( void )
{
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
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewInit();
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //************************************************************
    
    // Our triangle - An array of 3 vectors which represents 3 vertices
    float vertices1[] = {
        -0.7f, -0.7f, 0.0f,  //left
        0.57f, -0.7f, 0.0f,  //right
        -0.7f,  0.7f, 0.0f,  //top
        0.7f,  0.7f, 0.0f,  //top
        
    };

    float vertices2[] = {
        -0.1f, -0.1f, 0.0f,  //left
        0.1f, -0.1f, 0.0f,  //right
        -0.1f,  0.1f, 0.0f,  //top
        0.1f,  0.1f, 0.0f,  //top
        
    };
    
    //**************** CREATING VAO and VBO **********************
    unsigned int vertexArrayID1;  // id for vertex array object
    glGenVertexArrays(1, &vertexArrayID1);
    unsigned int vertexBuffer1;  // id for vertex buffer object
    glGenBuffers(1, &vertexBuffer1);
    
    glBindVertexArray(vertexArrayID1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer1);
    
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //*****************************************************

    unsigned int vertexArrayID2;  // id for vertex array object
    glGenVertexArrays(1, &vertexArrayID2);
    unsigned int vertexBuffer2;  // id for vertex buffer object
    glGenBuffers(1, &vertexBuffer2);
    
    glBindVertexArray(vertexArrayID2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2);
    
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    //************* VERTEX SHADER *************************
    // the simplest vertex shader program
    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //*****************************************************
    
    //************* FRAGMENT SHADER *************************
    // the simplest fragment shader program
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
    
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //*****************************************************
    
    //************* LINKING SHADERS *************************
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //*****************************************************
        
    // render loop
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS){
        
        // background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw our first triangles
        glUseProgram(shaderProgram);

        //*********************
        glBindVertexArray(vertexArrayID1);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertices1));
        

        glBindVertexArray(vertexArrayID2);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(vertices2));
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}
