
#include "MShader.hpp"

MShader::MShader(const char* vPath, const char* fPath) {
	std::string vCode, fCode;
	std::ifstream vFile, fFile;
	vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vFile.open(vPath);
		fFile.open(fPath);
		std::stringstream vStream, fStream;
		vStream << vFile.rdbuf();
		fStream << fFile.rdbuf();
		vFile.close();
		fFile.close();
		vCode = vStream.str();
		fCode = fStream.str();
	}
	catch(std::ifstream::failure& er) {
		std::cout << "Shader not read: " << std::endl;
	}
	const char* vShaderCode = vCode.c_str();
	const char* fShaderCode = fCode.c_str();

	unsigned int v, f;
	v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &vShaderCode, nullptr);
	glCompileShader(v);

	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f, 1, &fShaderCode, nullptr);
	glCompileShader(f);

	id = glCreateProgram();
	glAttachShader(id, f);
	glAttachShader(id, v);
	glLinkProgram(id);

	int  pass;
	char bruh[512];
	glGetProgramiv(id, GL_LINK_STATUS, &pass);
	if (!pass) {
		glGetProgramInfoLog(id, 512, NULL, bruh);
		std::cout << bruh;
	}

	glDeleteShader(v);
	glDeleteShader(f);
}

void MShader::Use() {
	glUseProgram(id);
}
void MShader::Delete() {
	glDeleteProgram(id);
}

void MShader::Set(const std::string& name, bool v) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)v);
}
void MShader::Set(const std::string& name, int v) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), v);
}
void MShader::Set(const std::string& name, unsigned int v) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), v);
}
void MShader::Set(const std::string& name, float v) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), v);
}
void MShader::Set(const std::string& name, glm::mat4& v) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
}
void MShader::Set(const std::string& name, glm::vec3 v) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(v));
}
