
#include "MCamera.h"
#include <algorithm>

glm::vec3 MCamera::LookVector() {
	return lVector;
	//return glm::vec3{ glm::normalize(target - position) };
}

// right hand rule
glm::vec3 MCamera::RightVector() {
	return glm::vec3{ glm::normalize(glm::cross(LookVector(), glm::vec3{0.0f, 1.0f, 0.0f})) };
}

glm::vec3 MCamera::UpVector() {
	return glm::cross(RightVector(), LookVector()); // Already unit vectors so it shouldn't matter
}

glm::vec3 MCamera::GetPosition() {
	return position;
}

void MCamera::LookAround(double x, double y) {
	double xDif{ x - lastX };
	double yDif{ lastY - y };
	lastX = x;
	lastY = y;
	xDif *= sensitivity;
	yDif *= sensitivity;
	yaw += xDif;
	pitch = std::max(-89.0, std::min(89.0, pitch + yDif));

	glm::vec3 direction;
	direction.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	direction.y = std::sin(glm::radians(pitch));
	direction.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	lVector = glm::normalize(direction);
}

void MCamera::MoveForward(int magnitude, float movementSpeed) {
	position += (magnitude * movementSpeed) * LookVector();
}

void MCamera::MoveLateral(int magnitude, float movementSpeed) {
	position += glm::normalize(glm::cross(LookVector(), UpVector())) * (movementSpeed * magnitude);
}

void MCamera::MoveVertical(int magnitude, float movementSpeed) {
	position += (magnitude * movementSpeed) * UpVector();
}
