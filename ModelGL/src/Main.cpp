
#define MY_USE_TEXTURE_ENUM

#include <iostream>

#include "mine/MWindow.h"
#include "mine/MAssimp.h"

const unsigned int WIDTH{ 1920 };
const unsigned int HEIGHT{ 1080 };

int main() {
	MWindow window{ WIDTH, HEIGHT };

	MY::Texture Aries{ "data/textures/aries.png", true, MY_DIFFUSE };

	//MY::Vertex BRUH{ glm::vec3{1.f}, glm::vec3{1.f}, glm::vec3{1.f} };

	while (window.Running()){
		
		glfwPollEvents();
	}
}
