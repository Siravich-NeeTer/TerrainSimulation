#pragma once

#include <glfw/glfw3.h>
#include <iostream>
#include <cstring>

namespace Input
{
	extern float mouseX;
	extern float mouseY;
	extern bool keyPressed[GLFW_KEY_LAST];
	extern bool keyBeginPressed[GLFW_KEY_LAST];

	bool isKeyPressed(const int& key);
	bool isKeyBeginPressed(const int& key);

	void endFrame();

	void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	void cursorCallBack(GLFWwindow* window, double xPos, double yPos);
	void mouseCallBack(GLFWwindow* window, int key, int action, int mods);
};