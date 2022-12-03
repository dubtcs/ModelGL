
#include <iostream>

#include "mine/allinclude.h"

const unsigned int WIDTH{ 1920 };
const unsigned int HEIGHT{ 1080 };

int main() {
	MWindow window{ WIDTH, HEIGHT };
	while (window.Running()){
		
		glfwPollEvents();
	}
}
