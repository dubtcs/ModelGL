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
private:
	static void FrameBufferResize(GLFWwindow* window, int w, int h);
	static void HandleInput(GLFWwindow* win);
	static void MouseMovement(GLFWwindow* w, double x, double y);
public:
	unsigned int width, height;
private:
	GLFWwindow* window;
};
