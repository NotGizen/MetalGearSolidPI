#pragma once
#include "IInput.h"
#include "Input.h"
#include <X11/Xlib.h>
class Camera;
class RaspMouse : public IMouse
{
public:
	RaspMouse(Display& display, Window& window);

	bool GetButtonDown(MouseButtons button) const override;
	glm::vec2 GetPosition() const override;
	glm::vec2 GetPosition(bool value) const override;
	float GetScrollDelta() const override;
	Display& display ;
	Window& window ; 
	

	

private:
};

