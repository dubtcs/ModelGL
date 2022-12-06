#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MCamera.h"

class MWindow {
public:
	MWindow(const unsigned int& w, const unsigned int& h);
	~MWindow();
public:
	bool Running();
	GLFWwindow*& GetWindow();
	MCamera& GetCamera();
	float GetAspectRatio();
private:
	static void FrameBufferResize(GLFWwindow* window, int w, int h);
	static void HandleInput(GLFWwindow* win);
	static void MouseMovement(GLFWwindow* w, double x, double y);
	static void KeyInput(GLFWwindow*, int key, int scancode, int action, int mods);
	static void MouseInput(GLFWwindow* window, int button, int action, int mods);
	static void ScrollInput(GLFWwindow* window, double xOffset, double yOffset);
public:
	unsigned int width, height;
private:
	float aspectRatio;
	GLFWwindow* window;
};
