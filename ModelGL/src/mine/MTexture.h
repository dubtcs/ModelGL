#pragma once

#include <glad/glad.h>
#include <string>

class MTexture {
public:
	MTexture(const std::string& path, GLenum s, bool isRGBA);
public:
	void Bind();
	void Bind(GLenum);
	static void StripSlot(GLenum s); // E MEEEE
private:
	GLenum bindSlot;
	int texWidth, texHeight, colorChannels;
	unsigned int id;
};
