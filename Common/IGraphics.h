// a simple interface class its basically just simple access to graphics
#pragma once
#ifdef Raspberry_BUILD
#include "XWindow.h"
#endif // Raspberry_BUILD
#ifdef WINDOWS_BUILD
#include <GLFW/glfw3.h>
#endif

class XWindow;
class IGraphics
{
public:
	virtual ~IGraphics() = default;
	virtual void SwapBuffer() = 0;
	virtual void Quit() = 0;
	
	//virtual XWindow& Window() = 0;
	virtual void MouseWrap() = 0;
	virtual void SetIsWindowsFocused(bool value) = 0;
};

