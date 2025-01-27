#include "WindowsGraphics.h"
#include "Game.h"
#include <iostream>
#include <glad/glad.h>
#include "WindowsInput.h"


void WindowsGraphics::SwapBuffer()
{
	glFlush();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

GLFWwindow& WindowsGraphics::WindowPC() 
{
	return *window;
}

//XWindow& WindowsGraphics::Window()
//{
//	
//}


void WindowsGraphics::MouseWrap()
{
}

void WindowsGraphics::SetIsWindowsFocused(bool value)
{
	if (value)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

WindowsGraphics::WindowsGraphics()
{
	// Initialize GLFW and set window properties.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // we need a base OpenGL 3.3 to emulate ES, otherwise use 3.1 for ES
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // for normal opengl
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	// Creates the window.
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PC Based OpenGLES", NULL, NULL);

	// Error handling for if window creation failed.
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
	}

	// Set the window to be the current context.
	glfwMakeContextCurrent(window);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);//DIsable mouse view
	

	// Error handling for if GLAD failed to initialize.
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
	}
}

void WindowsGraphics::Quit()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
