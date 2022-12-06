#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MCamera {
public:
	MCamera(glm::vec3 p) : position{ p } {};
public:
	glm::vec3 LookVector();
	glm::vec3 RightVector();
	glm::vec3 UpVector();
	glm::vec3 GetPosition();
	void MoveForward(int magnitude, float movementSpeed);
	void MoveLateral(int magnitude, float movementSpeed);
	void MoveVertical(int magnitude, float movementSpeed);
	void LookAround(double x, double y);
	bool ToggleLock();
	bool CanMove();
	void ChangeSpeed(double change);
private:
	double speed{ 1.0 };
	bool firstMouseMovement{ true };
	bool canMove{ false };
	glm::vec3 lVector{ 0.0 };
	float sensitivity = 0.5f;
	glm::vec3 position;
	double lastX{ 400 }, lastY{ 300 };
	double yaw{ 0 }, pitch{ 0 }, roll{ 0 };
};
