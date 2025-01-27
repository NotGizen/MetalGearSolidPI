#include "Input.h"

#include "IInput.h"
#include "Common.h"
#include "Camera.h"

Camera* Input::camera = nullptr;


Input::Input(const IKeyboard* const keyboard, const IMouse* const mouse) :
	mouse(mouse),
	keyboard(keyboard)
{
}

Input::~Input()
{
	delete(mouse);
	delete(keyboard);
}


const IKeyboard& Input::GetKeyboard() const
{
	return *keyboard;
}

const IMouse& Input::GetMouse() const
{
	return *mouse;
}

void Input::SetCameraReference(Camera* cam)
{
	camera = cam;
}
