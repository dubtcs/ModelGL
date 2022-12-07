
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
	MShader borderShader{ "src/shaders/border.vert", "src/shaders/border.frag" };
	MY::Model model{ "data/model/backpack.obj" };

	float fieldOfView{ 90.f };

	glm::mat4 UNIT{ 1.f };
	glm::mat4 toWorldSpace{ glm::rotate(UNIT, 0.f, glm::vec3{0.f, 0.f, 0.f,}) };
	glm::mat4 toViewSpace{ glm::translate(UNIT, glm::vec3{0.f, 0.f, -2.f}) };
	glm::mat4 toClipSpace{ glm::perspective(glm::radians(fieldOfView), (float)(window.GetAspectRatio()), 0.1f, 100.0f) };

	// Stencil buffer highlights
	float scale{ 2.f };
	glm::vec3 scaleVec{ scale, scale, scale };

	while (window.Running()) {
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		model.Draw(shader);

		// Outline
		borderShader.Use();
		borderShader.Set("toWorldSpace", toWorldSpace);
		borderShader.Set("toViewSpace", toViewSpace);
		borderShader.Set("toClipSpace", toClipSpace);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		model.Draw(borderShader);

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glfwPollEvents();
	}

	return EXIT_SUCCESS;
}
