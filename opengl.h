/*******************************************************************
**   File: opengl.h
** Author:	Paul Allen
**
** The opengl library provides a cross platform framework for
** a graphical windowed program environment. Its main purpose 
** is to be used in Software Engineering classes at FGCU. The
** library is based on code presented by LearnOpenGL.com.
******************************************************************/
#ifndef OPENGL_H
#define OPENGL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

enum GL_KEY {
	KEY_NONE,
	KEY_ESC,
	KEY_X,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_UP_ARROW,
	KEY_LEFT_ARROW,
	KEY_DOWN_ARROW,
	KEY_RIGHT_ARROW
};

struct RGBA {
	float red;
	float green;
	float blue;
	float alpah;
};

class OpenGL {
public:
	OpenGL(){}
	OpenGL(int width, int height, const char* title, bool borders=false, int margin=0);
	~OpenGL() { glfwTerminate(); }

	GL_KEY GetKey();
	float getTime();
	void pollEvents();

	bool isClosing() { return glfwWindowShouldClose(this->window); };

	void clearWindow(const unsigned char* color = 0);
	void paintWindow();

	void DrawSprite(const float* position, const float* size, const unsigned char* color = 0, float rotate = 0.0f, bool rotateCenter = true);

	//-----------------------------------------------------------------------------
	// depreciated  functions
	//-----------------------------------------------------------------------------
	void DrawSprite(float position_x, float position_y, float size_x, float size_y,
		int color_r = 1.0f, int color_g = 1.0f, int color_b = 1.0f);

	void DrawSprite(float position_x, float position_y, float size_x, float size_y, float rotate = 0.0f,
		float color_r = 1.0f, float color_g = 1.0f, float color_b = 1.0f);

private:
	GLFWwindow* window;
	unsigned int shaderProgram;
	unsigned int renderData;

};

#endif OPENGL_H