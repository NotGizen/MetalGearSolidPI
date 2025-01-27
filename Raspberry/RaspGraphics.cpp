#include "RaspGraphics.h"
#include <EGL/egl.h>
#include "EGLState.h"
#include "RaspMouse.h"
#include "XWindow.h"
#include "Common.h"

RaspGraphics::RaspGraphics() : window(new XWindow())
{
	window->CreateWindow();
}

void RaspGraphics::Quit()
{
	XDestroyWindow(&window->GetDisplay(), window->GetWindow());
}

void RaspGraphics::SwapBuffer()
{
	EGLState state = window->GetState();
	eglSwapBuffers(state.display, state.surface);
}


XWindow& RaspGraphics::Window() 
{
	return *window;
}

void RaspGraphics::MouseWrap()
{
	XWarpPointer(&Window().GetDisplay(), None, Window().GetWindow(), 0, 0, 0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	XFlush(&Window().GetDisplay());
}

void RaspGraphics::SetIsWindowsFocused(bool value)
{
	window->SetIsHidingCursor(value);
	window->UpdateCursorFocus();
}
