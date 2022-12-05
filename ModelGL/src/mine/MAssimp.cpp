
#include "MAssimp.h"
#include "../extern/stb_image.h"

namespace MY {

	void Mesh::SetupMesh() {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); // positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); // normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords)); // texture coordinates
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	void Mesh::Draw(MShader& shader) {
		unsigned int diffuseIndex{ 0 }, specularIndex{ 0 };
		for (unsigned int i{ 0 }; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			std::string materialIndex;
			TextureType& bruh{ textures[i].type };
			if (bruh == MY::TextureType::DIFFUSE) {
				materialIndex = ("diffuse") + std::to_string(diffuseIndex++); // ++ is after string call
			}
			else if (bruh == MY::TextureType::SPECULAR) {
				materialIndex = ("specular") + std::to_string(specularIndex++);
			}
			shader.Set(("materialInfo." + materialIndex), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	unsigned int GenerateTextureFromFile(const char* fileName, std::string& directory) {
		unsigned int id;

		directory = directory + '/' + fileName;
		glGenTextures(1, &id);

		int width, height, colorChannels;
		unsigned char* imageData = stbi_load(directory.c_str(), &width, &height, &colorChannels, 0);
		if (imageData) {
			GLenum imageFormat;
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

	void Model::Draw(MShader& shader) {
		for (Mesh& mesh : meshes) {
			mesh.Draw(shader);
		}
	}

	void Model::LoadModel(std::string filePath) {
		Assimp::Importer importer;
		const aiScene* scene{ importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs) };
		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
			std::cout << "Model scene not created uh oh stinky: " << importer.GetErrorString() << std::endl;
			return;
		}
		directory = filePath.substr(0, filePath.find_last_of('/'));
		std::cout << directory;
		ProcessNode(scene->mRootNode, scene);
		return;
	}

	// Convert node data into readable meshes and textures
	void Model::ProcessNode(aiNode* node, const aiScene* scene) {
		for (int i{ 0 }; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene));
		}
		for (int i{ 0 }; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
		return;
	}

	// Push aiMesh data into Mesh data for OpenGL
	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		for (int i{ 0 }; mesh->mNumVertices; i++) {
			aiVector3D& vertInfo = mesh->mVertices[i];
			glm::vec3 position{ vertInfo.x, vertInfo.y, vertInfo.z };

			glm::vec3 normal{ 0.f };
			if (mesh->HasNormals()) {
				aiVector3D normInfo = mesh->mNormals[i];
				normal = glm::vec3{ normInfo.x, normInfo.y, normInfo.z };
			}

			glm::vec2 textureCoords{ 0.f, 0.f };
			if (mesh->mTextureCoords[0]) {
				aiVector3D& texInfo{ mesh->mTextureCoords[0][i] };
				textureCoords = { texInfo.x, texInfo.y };
			}

			Vertex vert{ position, normal, textureCoords };
			vertices.push_back(vert);
		}
		for (int i{ 0 }; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (int b{ 0 }; b < face.mNumIndices; b++) {
				indices.push_back(face.mIndices[b]);
			}
		}
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial*& matInfo = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<Texture> diffuseMaps = LoadMaterialTextures(matInfo, aiTextureType_DIFFUSE, MY::TextureType::DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<Texture> specularMaps = LoadMaterialTextures(matInfo, aiTextureType_SPECULAR, MY::TextureType::SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		return Mesh{ vertices, indices, textures };
	}

	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType typeName) {
		std::vector<Texture> textures;
		for (int i{ 0 }; i < material->GetTextureCount(type); i++) {
			aiString str;
			material->GetTexture(type, i, &str);
			bool isLoaded{ false };
			for (int b{ 0 }; b < loadedTextures.size(); b++) {
				if (std::strcmp(loadedTextures[b].path.data(), str.C_Str()) == 0) {
					textures.push_back(loadedTextures[b]);
					isLoaded = true;
					break;
				}
			}
			if (!isLoaded) {
				unsigned int nid{ GenerateTextureFromFile(str.C_Str(), directory) };
				Texture tex{ nid, typeName };
				textures.push_back(tex);
			}
		}
		return textures;
	}

} // END MY
