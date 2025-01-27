#include "RaspMouse.h"


constexpr unsigned int WINDOW_WIDTH = 1024;
constexpr unsigned int WINDOW_HEIGHT = 768;
bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2.f;
float lastY = WINDOW_HEIGHT / 2.f;

RaspMouse::RaspMouse(Display& display, Window& window) :
	display(display),
	window(window)
{
}

bool RaspMouse::GetButtonDown(MouseButtons button) const
{
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(
		&display,
		window,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	switch (button) 
	{
		case MouseButtons::LEFT:
		{
			return mask_return & Button1MotionMask;
		}
		case MouseButtons::RIGHT:
		{
			return mask_return & Button3MotionMask;
		}
		case MouseButtons::MIDDLE:
		{
			return mask_return & Button2MotionMask;
		}
		default:
		{
			return false;
		}
	}
	
}

glm::vec2 RaspMouse::GetPosition() const
{
	return glm::vec2();
}

glm::vec2 RaspMouse::GetPosition(bool value) const
{
	int root_x;
	int root_y;
	int win_x;
	int win_y;
	uint mask_return;
	Window window_returned;
	XQueryPointer(
		&display,
		window,
		&window_returned,
		&window_returned,

		&root_x,
		&root_y,
		&win_x,
		&win_y,
		&mask_return);

	// Ignore the artificial mouse warp event
	if (!value && win_x == WINDOW_WIDTH / 2 && win_y == WINDOW_HEIGHT / 2)
	{
		
		return glm::vec2(lastX, lastY);
	}

	return glm::vec2{win_x, win_y};
}

float RaspMouse::GetScrollDelta() const
{
	return 0;
}




