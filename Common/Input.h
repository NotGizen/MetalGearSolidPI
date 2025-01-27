#pragma once


class IMouse;
class IKeyboard;
class Camera;
class Input
{
public:
	Input(const IKeyboard* const keyboard, const IMouse* const mouse);
	~Input();

	const IKeyboard& GetKeyboard() const;
	const IMouse& GetMouse() const;
	void SetCameraReference(Camera* cam);
	static Camera* camera;
	

private:
	const IMouse* const mouse;
	const IKeyboard* const keyboard;
};

