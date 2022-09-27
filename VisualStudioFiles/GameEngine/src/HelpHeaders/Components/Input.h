#pragma once

#include "../Component.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

#include <unordered_map>

enum class KeyCode {
	SPACE = 32, APOSTROPHE, COMMA = 44, MINUS, PERIOD, SLASH,																						// 6
	ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,																						// 10
	SEMICOLON = 59, EQUAL = 61,																														// 2
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,																// 26
	LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET, GRAVE_ACCENT = 96,																						// 4
	WORLD_1 = 161, WORLD_2,																															// 2
	ESCAPE = 256, ENTER, TAB, BACKSPACE, INSERT, DELETE,																							// 6
	RIGHT_ARROW, LEFT_ARRORW, DOWN_ARROW, UP_ARROW,																									// 4
	PAGE_UP, PAGE_DOWN, HOME, END,																													// 4
	CAPS_LOCK = 280, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,																					// 5
	F1 = 290, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,						// 25
	KP_0 = 320, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9, KP_DECIMAL, KP_DIVIDE, KP_MULTIPLY, KP_SUBTRACT, KP_ADD, KP_ENTER, KP_EQUAL,	// 17
	LEFT_SHIFT = 340, LEFT_CTRL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT, RIGHT_CTRL, RIGHT_ALT, RIGHT_SUPER, MENU										// 9 
};

enum class MouseButtonCode {
	LEFT,
	RIGHT
};


class Input : public Component<Input>
{
public:

	Input() = default;
	Input(const Input&) = delete;

	static GLFWwindow* gameWindow;

	float mouseDeltaX = 0;
	float mouseDeltaY = 0;

	double lastMousePosX = 0;
	double lastMousePosY = 0;

	double mousePosX = 0;
	double mousePosY = 0;

	float mouseScrollDelta = 0;

	static const unsigned int numKeyCodes = 120;
	static std::unordered_map<KeyCode, bool> keyDown;
	static std::unordered_map<MouseButtonCode, bool> mouseButtonDown;
	static KeyCode keyCodesVec[];

	bool KeyUp(KeyCode key);
	bool KeyDown(KeyCode key);
	bool KeyHeld(KeyCode key);

	bool MouseButtonUp(MouseButtonCode button);
	bool MouseButtonDown(MouseButtonCode button);
	bool MouseButtonHeld(MouseButtonCode button);


	bool MouseScrollDown() {
		return instance.mouseScrollDelta < 0;
	}

	bool MouseScrollUp() {
		return instance.mouseScrollDelta > 0;
	}

	void SetMouseVals();

	void Reset();

	static Input& Get() {
		return instance;
	}

	static bool cursorVisible;

	virtual void SerializeComponent() override {}
	virtual void DeSerializeComponent() override {}
	virtual void UIfyComponent() override {}


private:
	bool firstMouse = false;
	static Input instance;
};

