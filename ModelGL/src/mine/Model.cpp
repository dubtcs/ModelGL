
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../extern/stb_image.h"

#include <glad/glad.h>

using std::endl;

namespace MY {

	unsigned int LoadTexture(const char* fileName, std::string directory) {
		unsigned int id;
		stbi_set_flip_vertically_on_load(true);
		std::string fullPath{ directory + '/' + std::string(fileName) };
		glGenTextures(1, &id);
		std::cout << fullPath << endl;
		int width, height, colorChannels;
		unsigned char* imageData = stbi_load(fullPath.c_str(), &width, &height, &colorChannels, 0);
		if (imageData) {
			GLenum imageFormat{ GL_RED };
			if (colorChannels == 1)
				imageFormat = GL_RED;
			else if (colorChannels == 3)
				imageFormat = GL_RGB;
			else if (colorChannels == 4)
				imageFormat = GL_RGBA;
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			std::cout << "Texture fail;ed to fdaolsd!!!!!!!  " << directory << std::endl;
		}
		stbi_image_free(imageData);

		return id;
	}

	// MESH

	// Generates buffers and loads them into memory
	void Mesh::InitMesh() {
		// Vertex array
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		// Vertex buffer
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// Element buffer
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		// Attribute pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); // Can do this because the struct is just "inline" chunks of memory
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

		glBindVertexArray(0);
	}

	// Draw the mesh to the screen
	void Mesh::Draw(MShader& shader) {
		unsigned int diffuseAmount{ 1 }, specularAmount{ 1 };

		for (int i{ 0 }; i < textures.size(); i++) {
			Texture currentTexture{ textures[i] };
			glActiveTexture(GL_TEXTURE0 + i);

			std::string setString{ "material." };
			if (currentTexture.type == TextureType::DIFFUSE) {
				setString = setString + "Diffuse" + std::to_string(diffuseAmount++);
			}
			else {
				setString = setString + "Specular" + std::to_string(specularAmount++);
			}
			shader.Set("texture" + setString, i);
			glBindTexture(GL_TEXTURE_2D, currentTexture.id);
		}
		glActiveTexture(0);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	// END MESH

	// MODEL

	// Draw the model to the screen
	void Model::Draw(MShader& shader) {
		for (Mesh& mesh : meshes) {
			mesh.Draw(shader);
		}
	}

	void Model::LoadModel(std::string path) {
		Assimp::Importer importer;
		const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
			std::cout << "Scene is munted: " << importer.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/')); // Remove the file name for texture 
		HandleNode(scene->mRootNode, scene);
	}

	void Model::HandleNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i{ 0 }; i < node->mNumMeshes; i++) {
			aiMesh* currentMesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(HandleMesh(currentMesh, scene));
		}
		for (unsigned int i{ 0 }; i < node->mNumChildren; i++) {
			aiNode* nextNode{ node->mChildren[i] };
			HandleNode(nextNode, scene);
		}
	}

	Mesh Model::HandleMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> verts;
		std::vector<unsigned int> inds;
		std::vector<Texture> texts;
		// Vertices
		for (int i{ 0 }; i < mesh->mNumVertices; i++) {
			aiVector3D& meshVert = mesh->mVertices[i];
			Vertex currentVertex{};
			currentVertex.position = glm::vec3{ meshVert.x, meshVert.y, meshVert.z };
			aiVector3D& meshNormal = mesh->mNormals[i];
			currentVertex.normal = glm::vec3{ meshNormal.x, meshNormal.y, meshNormal.z };
			if (mesh->mTextureCoords[0] != nullptr) {
				aiVector3D& meshTexCoords = mesh->mTextureCoords[0][i];
				currentVertex.textureCoordinates = glm::vec2{ meshTexCoords.x, meshTexCoords.y };
			}
			else {
				std::cout << "NOTEXT COORDS HEHEHEHEHE!!!" << endl;
				currentVertex.textureCoordinates = glm::vec2{ 0.f, 0.f };
			}
			//std::cout << currentVertex.textureCoordinates.x << ", " << currentVertex.textureCoordinates.y << endl;
			verts.push_back(currentVertex);
		}
		// Indices
		for (unsigned int i{ 0 }; i < mesh->mNumFaces; i++) {
			aiFace& currentFace{ mesh->mFaces[i] };
			for (unsigned int b{ 0 }; b < currentFace.mNumIndices; b++) {
				inds.push_back(currentFace.mIndices[b]);
			}
		}
		// Materials and textures
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial*& currentMaterial = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseTextures = LoadTextures(currentMaterial, aiTextureType_DIFFUSE, MY::TextureType::DIFFUSE);
			texts.insert(texts.end(), diffuseTextures.begin(), diffuseTextures.end());
			std::vector<Texture> specularTexture = LoadTextures(currentMaterial, aiTextureType_SPECULAR, MY::TextureType::SPECULAR);
			texts.insert(texts.end(), specularTexture.begin(), specularTexture.end());
		}
		return Mesh{ verts, texts, inds };
	}

	// Can probably change this to static
	std::vector<Texture> Model::LoadTextures(aiMaterial* material, aiTextureType textureType, MY::TextureType TYPE) {
		std::cout << "Loading texture" << endl;
		std::vector<Texture> textureList;
		for (unsigned int i{ 0 }; i < material->GetTextureCount(textureType); i++) {
			aiString str;
			material->GetTexture(textureType, i, &str);
			std::cout << str.C_Str() << endl;
			bool shouldSkip{ false };
			for (unsigned int b{ 0 }; b < loadedTextures.size(); b++) {
				if (loadedTextures[b].path == directory) {
					textureList.push_back(loadedTextures[b]);
					shouldSkip = true;
					break;
				}
			}
			if(!shouldSkip) {
				Texture texture{};
				texture.id = LoadTexture(str.C_Str(), directory);
				texture.type = TYPE;
				texture.path = directory;
				textureList.push_back(texture);
				loadedTextures.push_back(texture); // should probs use pointers at some point
			}
		}
		return textureList;
	}

}
