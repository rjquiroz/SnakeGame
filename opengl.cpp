/******************************************************************************
**   File: opengl.cpp
** Author:	Paul Allen
**
** The opengl library provides a cross platform framework for
** a graphical windowed program environment. Its main purpose
** is to be used in Software Engineering classes at FGCU. The
** library is based on code presented by LearnOpenGL.com.
*******************************************************************************/
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "opengl.h"


//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
// Shader function declerations
void initShaders(unsigned int &shaderID);
void checkCompileErrors(unsigned int object, std::string type);
// set screen cooridnate projection
void setProjection(unsigned int shaderID, int width, int height);
// Render data for a rectangle
void initRenderData(unsigned int &data);
// convert colors from bytes to floats
RGBA convertColor(const unsigned char* color);

// GLFW function declerations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//-----------------------------------------------------------------------------
// global constants
//-----------------------------------------------------------------------------
// vertex shader source code
const char *VSHADER_SOURCE = "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * model * vec4(aPos, 1.0);\n"
    "}\n";

// fragment shader source code
const char *FSHADER_SOURCE = "#version 330 core\n"
    "out vec4 color;\n"
    "uniform vec3 spriteColor;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(spriteColor, 1.0);\n"
    "}\n";


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------
OpenGL::OpenGL(int width, int height, const char *title, bool borders, int margin)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    this->window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initShaders(this->shaderProgram);

    setProjection(this->shaderProgram, width, height);

    initRenderData(this->renderData);
}


//-----------------------------------------------------------------------------
// member methods
//-----------------------------------------------------------------------------

/******************************************************************************
 returns key board key codes if a key is pressed when polled
 parameters:
 returns:
    GL_KEY  - key pressed or KEY_NONE
 ******************************************************************************/
GL_KEY OpenGL::GetKey()
{
    GL_KEY key = KEY_NONE;

    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        key = KEY_ESC;
    else if (glfwGetKey(this->window, GLFW_KEY_X) == GLFW_PRESS)
        key = KEY_X;
    else if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        key = KEY_W;
    else if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        key = KEY_A;
    else if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        key = KEY_S;
    else if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        key = KEY_D;
    else if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)
        key = KEY_UP_ARROW;
    else if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        key = KEY_LEFT_ARROW;
    else if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        key = KEY_DOWN_ARROW;
    else if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        key = KEY_RIGHT_ARROW;

    return key;
}


float OpenGL::getTime()
{
    return glfwGetTime();
}

void OpenGL::pollEvents()
{
    glfwPollEvents();
}



/******************************************************************************
 clears the screen buffers and sets background color
 parameters:
    background  - RGB color values for background
 returns:
    void
 ******************************************************************************/
void OpenGL::clearWindow(const unsigned char *background)
{
    RGBA rgb = convertColor(background);

    glClearColor(rgb.red, rgb.green, rgb.blue, rgb.alpah);
    glClear(GL_COLOR_BUFFER_BIT);
}


/******************************************************************************
 flushes changes to the screen buffers to the window
 parameters:
 returns:
 	void
 ******************************************************************************/
void OpenGL::paintWindow()
{
    glfwSwapBuffers(this->window);
}


/******************************************************************************
 draws a rectangle to the screen buffers
 parameters:
 	position	- array of coordinates {left, top} of top-left corner
 	size_x      - array of {width, height} of sprite
 	rotate		- rotation of rectangle in radians
 	color 		- array of RGBA {alpha, blue, green, red } color values [(.01 - 1.0) or (0-255)]
 returns:
 	void
 ******************************************************************************/
void OpenGL::DrawSprite(const float* position, const float* size, const unsigned char* color, float rotate, bool rotateCenter)
{
    // prepare transformations
    glUseProgram(this->shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
    model = glm::translate(model, glm::vec3(*position, *(position+1), 0.0f));

    if (rotateCenter)
        model = glm::translate(model, glm::vec3(0.5f * (*size), 0.5f * (*(size+1)), 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    if (rotateCenter)
        model = glm::translate(model, glm::vec3(-0.5f * (*size), -0.5f * (*(size+1)), 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(*size, *(size+1), 1.0f)); // last scale
    // set position
    glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, "model"), 1, false, glm::value_ptr(model));

    RGBA rgb = convertColor(color);
    glUniform3f(glGetUniformLocation(this->shaderProgram, "spriteColor"), rgb.red, rgb.green, rgb.blue);

    // bind the triangles to create a rectangle
    glBindVertexArray(this->renderData);
    //    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

//-----------------------------------------------------------------------------
// depreciated  functions
//-----------------------------------------------------------------------------
// lab-1
void OpenGL::DrawSprite(float position_x, float position_y, float size_x, float size_y,
    int color_r, int color_g, int color_b)
{
    DrawSprite(position_x, position_y, size_x, size_y, 0.0f,
        color_r, color_g, color_b);
}

void OpenGL::DrawSprite(float position_x, float position_y, float size_x, float size_y, float rotate,
    float color_r, float color_g, float color_b)
{
    float position[2]{ position_x, position_y };
    float size[2]{ size_x, size_y };
    unsigned char color[4]{ 0, color_b, color_g, color_r };
    DrawSprite(position, size, color, rotate );
}


//-----------------------------------------------------------------------------
// private functions
//-----------------------------------------------------------------------------

/******************************************************************************
 initializes the vertex and fragment shaders
 parameters:
 	shaderID	- (out) shader program ID set during intialization
 returns:
 	void
 ******************************************************************************/
void initShaders(unsigned int &shaderID)
{
    unsigned int sVertex, sFragment;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &VSHADER_SOURCE, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &FSHADER_SOURCE, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // shader program
    shaderID = glCreateProgram();
    glAttachShader(shaderID, sVertex);
    glAttachShader(shaderID, sFragment);
    glLinkProgram(shaderID);
    checkCompileErrors(shaderID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}


/******************************************************************************
 displays compiler and linker errors to the console for shaders
 parameters:
 	object		- shader or program ID
 	type 		- string name of the shader type
 returns:
 	void
 ******************************************************************************/
void checkCompileErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}


/******************************************************************************
 sets a projection to scale screen width/height to viewport width/height
 parameters:
 	shaderID	- shader program ID set during intialization
 	width		- width of the screen
 	height		- height of the screen
 returns:
 	void
 ******************************************************************************/
void setProjection(unsigned int shaderID, int width, int height)
{
    glUseProgram(shaderID);
    //                                left  right                      bottom                      top
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, false, glm::value_ptr(projection));
}


/******************************************************************************
 sets the vertices of the triangles that make up the sprite rectangle
 parameters:
 	data	- (out) VAO buffer to hold the sprite data
 returns:
 	void
 ******************************************************************************/
void initRenderData(unsigned int &data) {
    float vertices[] = {
        // pos
        0.0f, 1.0f, 0.0f,    // bottom left
        1.0f, 0.0f, 0.0f,    // top right
        0.0f, 0.0f, 0.0f,    // top left
        1.0f, 1.0f, 0.0f,    // bottom right
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 1, 3
    };
    // configure VAO/VBO
    unsigned int VBO = 0, EBO = 0;
    // ..:: Initialization code :: ..
    glGenVertexArrays(1, &data);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 1. bind Vertex Array Object
    glBindVertexArray(data);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

/******************************************************************************
 convert color from byte array to float
 parameters:
    color		- byte array of color attributes
 returns:
    RGBA        - structure of float color values
 ******************************************************************************/
RGBA convertColor(const unsigned char* color) {
    RGBA out{ 1.0f, 1.0f, 1.0f, 1.0f };

    if (color) {
        if (*(color + 3) > 1.0f)
            out.red = *(color + 3) / 255.0f;
        else
            out.red = *(color + 3);

        if (*(color + 2) > 1.0f)
            out.green = *(color + 2) / 255.0f;
        else
            out.green = *(color + 2);

        if (*(color + 1) > 1.0f)
            out.blue = *(color + 1) / 255.0f;
        else
            out.blue = *(color + 1);
    }
    return out;
}

//-----------------------------------------------------------------------------
// GLFW function declerations
//-----------------------------------------------------------------------------

/******************************************************************************
 callback function that keep the viewport size in sync with changes user makes
 to the displayed window
 parameters:
 	window		- pointer to the GLFW window object
 	width		- new width of the window on the screen
 	height		- new height of the window on the screen
 returns:
 	void
 ******************************************************************************/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/******************************************************************************
 callback function that captures keys pressed on the keyboard
 parameters:
 	window		- pointer to the GLFW window object
 	key			- code for key that was pressed
 	scancode	- not implemented yet
 	action 		- tells us if key is pressed or raising
 	mode 		- not implemented yet
 returns:
 	void
 ******************************************************************************/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
