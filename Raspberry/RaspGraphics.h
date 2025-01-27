#pragma once
#include "IGraphics.h"
#include "XWindow.h"
class XWindow;
class GLFWwindow;
struct EGLState;

class RaspGraphics : public IGraphics
{
public:
	RaspGraphics();
	~RaspGraphics() {};

	void Quit() override;

	void SwapBuffer() override; // 
	//virtual GLFWwindow& Window() const override;
	XWindow& Window();
	void MouseWrap() override;
	void SetIsWindowsFocused(bool value) override;
private:
	XWindow* window;
};

