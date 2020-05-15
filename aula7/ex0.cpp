
// Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// Vertex shader
const GLchar* vertexShaderSrc = "#version 150 core \n"
"in float inValue; \n"
"out float outValue;\n"
"void main()\n"
"{outValue = sqrt(inValue); \n"
"}"


int main()
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
	    GLFWwindow* window = glfwCreateWindow( 1280, 720, "transf", NULL, NULL);
	    glfwMakeContextCurrent(window);
	    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	    // Ensure we can capture the escape key being pressed below
	    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Compile shader
        GLuint shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader, 1, &vertexShaderSrc, nullptr);
        glCompileShader(shader);

        // Create program and specify transform feedback variables
        GLuint program = glCreateProgram();
        glAttachShader(program, shader);

        const GLchar* feedbackVaryings[] = { "outValue" };
        glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

        glLinkProgram(program);
        glUseProgram(program);

        // Create VAO
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create input VBO and vertex format
        GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

        GLint inputAttrib = glGetAttribLocation(program, "inValue");
        glEnableVertexAttribArray(inputAttrib);
        glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

        // Create transform feedback buffer
        GLuint tbo;
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_READ);

        // Perform feedback transform
        glEnable(GL_RASTERIZER_DISCARD);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
        glBeginTransformFeedback(GL_POINTS);
            glDrawArrays(GL_POINTS, 0, 5);
        glEndTransformFeedback();
        glDisable(GL_RASTERIZER_DISCARD);
        glFlush();

        // Fetch and print results
        GLfloat feedback[5];
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

        printf("%f %f %f %f %f\n", feedback[0], feedback[1], feedback[2], feedback[3], feedback[4]);

        glDeleteProgram(program);
        glDeleteShader(shader);

        glDeleteBuffers(1, &tbo);
        glDeleteBuffers(1, &vbo);

        glDeleteVertexArrays(1, &vao);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
