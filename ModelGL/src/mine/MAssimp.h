#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "MShader.hpp"

#ifdef MY_USE_TEXTURE_ENUM
#define MY_DIFFUSE MY::TextureType::DIFFUSE
#define MY_SPECULAR MY::TextureType::SPECULAR
#endif // MY_USE_TEXTURE_ENUM

namespace MY {

	enum class TextureType {
		DIFFUSE,
		SPECULAR
	};

	struct Vertex {
	public:
		Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 tx) :position{ p }, normal{ n }, textureCoords{ tx } {}; // methods act as padding in the memory layout
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoords;
	};

	class Texture {
	public:
		Texture(const std::string& path, bool isRGBA, TextureType tt);
		TextureType type;
		unsigned int id;
	};

	class Mesh {
	public:
		Mesh(std::vector<Vertex>& verts, std::vector<unsigned int>& index, std::vector<Texture>& textures) : vertices{ verts }, indices{ index }, textures{ textures } { SetupMesh(); }
		void Draw(MShader& s);
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
	private:
		unsigned int vertexArray, vertexBuffer, elementBuffer;
		void SetupMesh();
	};

} // END MY
