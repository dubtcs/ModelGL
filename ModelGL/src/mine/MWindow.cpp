
#include <iostream>

#include "MWindow.h"

MCamera currentCamera{ glm::vec3{0.f,0.f,0.f} };
float dt, lastFrame;

MWindow::MWindow(const unsigned int& w, const unsigned int& h) : width{ w }, height{ h } {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	window = glfwCreateWindow(width, height, "WINDOWOWOW", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "No window blud" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD no init" << std::endl;
		glfwTerminate();
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, FrameBufferResize);
	glfwSetCursorPosCallback(window, MouseMovement);

	glEnable(GL_DEPTH_TEST);
}
MWindow::~MWindow() {
	glfwTerminate();
}

bool MWindow::Running() {
	HandleInput(window);
	glfwSwapBuffers(window);
	return !glfwWindowShouldClose(window);
}

GLFWwindow*& MWindow::GetWindow() {
	return window;
}

MCamera& MWindow::GetCamera() {
	return currentCamera;
}

void MWindow::FrameBufferResize(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}
void MWindow::HandleInput(GLFWwindow* window) {
	dt = lastFrame - glfwGetTime();
	lastFrame = glfwGetTime();
	float cameraSpeed = 1.f * dt;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		currentCamera.MoveForward(-1, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		currentCamera.MoveForward(1, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		currentCamera.MoveLateral(1, cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		currentCamera.MoveLateral(-1, cameraSpeed);
}
void MWindow::MouseMovement(GLFWwindow* w, double x, double y) {
	currentCamera.LookAround(x, y);
}
