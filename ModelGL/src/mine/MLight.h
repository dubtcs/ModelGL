#pragma once

// Was going to make light instances

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define ML_POINT LightType::POINT;
#define ML_DIRECTIONAL LightType::DIRECTIONAL;
#define ML_SPOT LightType::SPOT;

enum class LightType {
	POINT,
	DIRECTIONAL,
	SPOT
};

struct MLightConfig {
	MLightConfig() = delete;
	glm::vec3 position;
	glm::vec3 color;
};

struct MLightDirectionalConfig : MLightConfig {
public:
	MLightDirectionalConfig(int);
	glm::vec3 direction;
};

class MLight {
public:
	MLight(glm::vec3, LightType, MLightDirectionalConfig);
private:
	glm::vec3 position;
	
};
