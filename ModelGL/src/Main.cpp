
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

	float fieldOfView{ 90.f };

	glm::mat4 UNIT{ 1.f };
	glm::mat4 toWorldSpace{ glm::rotate(UNIT, 0.f, glm::vec3{0.f, 0.f, 0.f,}) };
	glm::mat4 toViewSpace{ glm::translate(UNIT, glm::vec3{0.f, 0.f, -2.f}) };
	glm::mat4 toClipSpace{ glm::perspective(glm::radians(fieldOfView), (float)(window.GetAspectRatio()), 0.1f, 100.0f) };

	while (window.Running()) {
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MCamera& cam = window.GetCamera();
		glm::vec3 cameraPos = cam.GetPosition();
		toViewSpace = glm::lookAt(cameraPos, cameraPos + cam.LookVector(), cam.UpVector());

		shader.Use();
		toWorldSpace = glm::translate(UNIT, glm::vec3{ 0.f, 0.f, 0.f });
		toWorldSpace = glm::rotate(toWorldSpace, (glm::radians(35.f) * (float)glfwGetTime()), glm::vec3{ 0.f, 2.f, 0.f }); // so it rotates
		toClipSpace = glm::perspective(glm::radians(fieldOfView), (float)(window.GetAspectRatio()), 0.1f, 100.0f);
		shader.Set("toWorldSpace", toWorldSpace);
		shader.Set("toViewSpace", toViewSpace);
		shader.Set("toClipSpace", toClipSpace);

		model.Draw(shader);

		glfwPollEvents();
	}

	return EXIT_SUCCESS;
}
