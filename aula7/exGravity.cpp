
// Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <chrono>
//#include <>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void checkCompileErrors(GLuint shader, std::string type);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// Vertex shader
const GLchar* vertexShaderSrc = "\n"
"#version 150 core\n"
"in vec2 position;\n"
"in vec2 velocity;\n"
"in vec2 originalPos;\n"
"out vec2 outPosition;\n"
"out vec2 outVelocity;\n"

"uniform float time;\n"
"uniform vec2 mousePos;\n"

"void main()\n"
"{\n"
 // Points move towards their original position...
"vec2 newVelocity = originalPos - position;\n"
"vec2 acceleration;\n"
// ... unless the mouse is nearby. In that case, they move towards the mouse.
    "if (length(mousePos - originalPos) < 0.75f) {\n"
        "acceleration = 1.5f * normalize(mousePos - position);\n"
        "newVelocity = velocity + acceleration*time;\n" // kinematics eq #TODO
    "}\n"
    "if (length(newVelocity) > 1.0f){\n"
        "newVelocity = normalize(newVelocity);"
        "}\n"
    "vec2 newPosition = position + newVelocity * time +acceleration*time*time;\n"  // kinematics eq #TODO
    "outPosition = newPosition;\n"
    "outVelocity = newVelocity;\n"
    "gl_Position = vec4(newPosition, 0.0, 1.0);\n"
"}\n";

// Fragment shader
const GLchar* fragmentShaderSrc = 
    "#version 150 core \n"
    "out vec4 outColor;\n"
    "void main(){\n"
        "outColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

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
	GLFWwindow* window = glfwCreateWindow( 800, 800, "transf", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Compile shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShader);


    // Create program and specify transform feedback variables
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    const GLchar* feedbackVaryings[] = { "outPosition", "outVelocity" };
    glTransformFeedbackVaryings(program, 2, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(program);
    checkCompileErrors(program, "PROGRAM");
    glUseProgram(program);

    GLint uniTime = glGetUniformLocation(program, "time");
    GLint uniMousePos = glGetUniformLocation(program, "mousePos");

    // Create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create input VBO and vertex format
    GLfloat data[600] = {};
    // Vertex format: 6 floats per vertex:
    // pos.x  pox.y  vel.x  vel.y  origPos.x  origPos.y

    // Set original and initial positions
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            data[60 * y + 6 * x] = 0.2f * x - 0.9f;
            data[60 * y + 6 * x + 1] = 0.2f * y - 0.9f;
            data[60 * y + 6 * x + 4] = 0.2f * x - 0.9f;
            data[60 * y + 6 * x + 5] = 0.2f * y - 0.9f;
        }
    }

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STREAM_DRAW);

    GLint posAttrib = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

    GLint velAttrib = glGetAttribLocation(program, "velocity");
    glEnableVertexAttribArray(velAttrib);
    glVertexAttribPointer(velAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    GLint origPosAttrib = glGetAttribLocation(program, "originalPos");
    glEnableVertexAttribArray(origPosAttrib);
    glVertexAttribPointer(origPosAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));

    // Create transform feedback buffer
    GLuint tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, 400 * sizeof(GLfloat), nullptr, GL_STATIC_READ);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
    GLfloat feedback[400];

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glPointSize(5.0f);
    auto t_prev = std::chrono::high_resolution_clock::now();

    while(!glfwWindowShouldClose(window)){
    	// input
		processInput(window);

		// Clear the screen to black
		 glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		 glClear(GL_COLOR_BUFFER_BIT);

    	// Calculate delta time
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float> >(t_now - t_prev).count();
		t_prev = t_now;
		glUniform1f(uniTime, time);

        // Update mouse position
        glUniform2f(uniMousePos, lastX / 400.0f - 1, -lastY / 400.0f + 1);

        // Perform feedback transform and draw vertices
        glBeginTransformFeedback(GL_POINTS);
            glDrawArrays(GL_POINTS, 0, 100);
        glEndTransformFeedback();

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Update vertices' position and velocity using transform feedback
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

        //#TODO
        for (int i = 0; i < 100; i++) {
            data[6 * i] = feedback[4*i];
            data[6 * i + 1] = feedback[4*i+1];
            data[6 * i + 2] = feedback[4*i+2];
            data[6 * i + 3] = feedback[4*i+3];
        }

        //new buffer with data
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), data);
    }

    glDeleteProgram(program);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

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

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

