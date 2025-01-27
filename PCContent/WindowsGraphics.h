#pragma once
/*This is where most of the PC set up content is contained, its very similar to Jacco's tmpl8
 *
 **/

#include <GLFW/glfw3.h>

#include "IGraphics.h"
#include "IInput.h"

//constexpr unsigned int WINDOW_WIDTH = 1024;
//constexpr unsigned int WINDOW_HEIGHT = 768;
//constexpr float ASPECT_RATIO = 1024.0f / 768.0f;

class WindowsGraphics : public IGraphics
{
public:
	WindowsGraphics();

	void Quit() override;
	void SwapBuffer() override;
	void SetIsWindowsFocused(bool value) override;
	GLFWwindow& WindowPC();
	//virtual XWindow& Window() override;
	virtual void MouseWrap() override;

private:
	GLFWwindow* window;
};

