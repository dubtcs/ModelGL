#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <string>

#include "MShader.hpp"

#ifdef MY_USE_TYPES
#define MY_DIFFUSE MY::TextureTypes::DIFFUSE
#define MY_SPECULAR MY::TextureTypes::SPECULAR
#endif // MY_USE_TYPES

namespace MY {

	enum class TextureType {
		DIFFUSE,
		SPECULAR
	};

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinates;
	};

	struct Texture {
		unsigned int id; // OpenGL texture id
		MY::TextureType type;
		std::string path;
	};

	unsigned int LoadTexture(const char*, std::string);

	class Mesh {
	public:
		Mesh(std::vector<Vertex> v, std::vector<Texture> t, std::vector<unsigned int> i) : vertices{ v }, textures{ t }, indices{ i } { InitMesh(); };
	public:
		void Draw(MShader& shader);
	private:
		std::vector<Vertex> vertices; // All vertices in mesh
		std::vector<Texture> textures; // All textures in mesh
		std::vector<unsigned int> indices; // Indices of vertices
		unsigned int vertexArray, vertexBuffer, elementBuffer; // bruh
		void InitMesh(); // Generates buffers and loads them into memory
	};

	class Model {
	public:
		Model(const char* path) { LoadModel(path); };
		void Draw(MShader& shader);
	private:
		void LoadModel(std::string path);
		void HandleNode(aiNode* node, const aiScene* scene);
		Mesh HandleMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadTextures(aiMaterial* material, aiTextureType textureType, MY::TextureType TYPE);
	private:
		std::vector<Texture> loadedTextures;
		std::vector<Mesh> meshes;
		std::string directory;
	};

}
