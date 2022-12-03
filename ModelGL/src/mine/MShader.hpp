#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class MShader {
public:
	MShader(const char* vPath, const char* fPath);
public:
	void Use();
	void Delete();
	void Set(const std::string& name, bool val) const;
	void Set(const std::string& name, int val) const;
	void Set(const std::string& name, float val) const;
	void Set(const std::string& name, glm::mat4& v) const;
	void Set(const std::string& name, glm::vec3 v) const;
public:
	unsigned int id;
};
