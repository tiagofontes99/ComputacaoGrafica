// http://www.opengl-tutorial.org/
// https://learnopengl.com/Getting-started/Hello-Triangle
// https://open.gl/drawing
#include "stb_image.h"
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);



int main( void )
{
    //************************** CONTEXT **********************************
    // glfw: initialize and configure
    if(!glfwInit()){
        std::cout << "Failed to initialize GLFW\n" << std::endl;
        std::cin.get();
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);  // anti-aliasing factor
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy;
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // use modern openGL
    
    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( 1024, 768, "A Triangle", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        std::cin.get();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //deal with window resize
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW\n";
        std::cin.get();
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //************************************************************
    
    // OS 3 PRIMEIROS SAO VERICES X,Y,Z E OS DOIS ULTIMOS CORRESPONDEM A TEXTIRA S, T;
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, //left
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, //right
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f, //top
    };

    


    
    //**************** CREATING VAO and VBO **********************
    unsigned int vertexArrayID;  // id for vertex array object
    glGenVertexArrays(1, &vertexArrayID);
    unsigned int vertexBuffer;  // id for vertex buffer object
    glGenBuffers(1, &vertexBuffer);
    
    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    

    //AQUI COMEÇA A DECLARAÇAO DO VBO 
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //texture coordinates
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //*****************************
    

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    //check for errors
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //*****************************************************
    Shader shader("vertex.vs", "fragment.fs");


    //vec rotation
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0,
        1.0)    );
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    //trans = glm::translate(trans, glm::vec3(x, y, z));
    //transformation
    unsigned int transformLoc = glGetUniformLocation(shader.ID,
        "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    //************* FRAGMENT SHADER *************************
/*
    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;"
    //"uniform vec4 ourColor;\n"
    "uniform sampler2D ourTexture;"// recebe o ID da textura gerada, através do BindTexture()
    "void main()\n"
    "{\n"
    //"    FragColor = ourColor;\n"
        "FragColor= texture=(ourTexture ,TexCoord);"
    "}\0";
    */
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    //check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //*****************************************************
    
    //************* LINKING SHADERS *************************
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    //check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //*****************************************************
    float timeValue = glfwGetTime();
    float aValue = (sin(timeValue) / 2.0f) + 0.5f;
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    Shader myShader("shader.vs" ,"shader.fs");

    // render loop
    while (!glfwWindowShouldClose(window)){

        timeValue = glfwGetTime();
        aValue = (sin(timeValue) / 2.0f) + 0.5f;

        int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        glUseProgram(shaderProgram); //é necessário activar o programa.
        glUniform4f(vertexColorLocation, aValue, 0.1f, 0.1f, 0.2f);

        processInput(window);

        
        
        // background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayID); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0); // no need to unbind it every time
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();



        //bind textura – goes to uniform sampler variable.
        glBindTexture(GL_TEXTURE_2D, texture);
        //bind VAO
        glBindVertexArray(vertexArrayID);
        //use shader program
        myShader.use();
        //draw
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    
    // Cleanup
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteProgram(shaderProgram);
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
