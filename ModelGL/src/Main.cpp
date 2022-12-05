
#define MY_USE_TEXTURE_ENUM
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "mine/MWindow.h"
#include "mine/Model.h"

const unsigned int WIDTH{ 1920 };
const unsigned int HEIGHT{ 1080 };

int main() {
	MWindow window{ WIDTH, HEIGHT };

	MShader shader{ "src/shaders/v1.vert", "src/shaders/f1.frag" };
	MY::Model model{ "data/model/backpack.obj" };

	while (window.Running()){
		
		glfwPollEvents();
	}
}
