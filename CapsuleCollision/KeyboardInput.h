// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// Author		:	Jasper Lyons
// Email		:	Jasper.Lyo7552@mediadesign.school.nz
// File Name	:	Input.h
// Description	:	Header file defining the Input class

#pragma once
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

#include <vector>
#include <memory>

enum InputState
{
	INPUT_FIRST_RELEASE, // First frame of Up state
	INPUT_RELEASED, // Default state (up)
	INPUT_FIRST_PRESS, // First frame of down state
	INPUT_HOLD // Key is held down
};

enum InputMouse
{
	MOUSE_LEFT,
	MOUSE_MIDDLE,
	MOUSE_RIGHT
};

class Input
{
#pragma region Singleton
public:
	static std::shared_ptr<Input> GetInstance();
	static void DestroyInstance();
	~Input();
private:
	static std::shared_ptr<Input> s_pInput;
	Input(Input const&);
	void operator=(Input const&);
	Input();

#pragma endregion

public:
	static unsigned int KeyState[255];
	static unsigned int MouseState[3];

	// Setters
	void SetCursor(int _cursorSetting);

	// Getters
	glm::vec2 GetMousePos();
	glm::vec2 GetMouseWorldPos();

	// Other Functions
	static void Initialize();
	static void Update();
	void ProcessNormalKeysDown(unsigned char _key, int _x, int _y);
	void ProcessNormalKeysUp(unsigned char _key, int _x, int _y);
	void ProcessSpecialKeys(int _key, int _x, int _y);
	void MouseButton(int _button, int _state, int _x, int _y);
	void MouseInput(int _x, int _y);

private:
	glm::vec2 m_MousePos;
};

