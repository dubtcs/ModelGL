
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

	Texture::Texture(const std::string& path, bool isRGBA, MY::TextureType tt) : type{ tt } {
		int texWidth, texHeight, colorChannels;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S,T,R coordinates
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* imageData = stbi_load(path.c_str(), &texWidth, &texHeight, &colorChannels, 0);
		if (imageData) {
			if (isRGBA)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "bruh";
		}
		stbi_image_free(imageData);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

} // END MY
