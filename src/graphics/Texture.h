#pragma once

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>

struct TextureData { // texture data
	unsigned char *pixels;
	int width, height, components;
};

class Texture {
public:
	Texture(const char *path) : path(path) {
		glGenTextures(1, &ID); // generate texture id
		data = LoadImageData(path); // retrieve image data
		if (data.pixels) {
			GLenum format = GL_RGBA;
			if (data.components == 3)
				format = GL_RGB;

			glBindTexture(GL_TEXTURE_2D, ID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, data.width, data.height, 0, format, GL_UNSIGNED_BYTE, data.pixels);
			// initialize mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			FreeImage(data.pixels); // free pixels
		}
	}


	void Bind() { // bind texture
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void Clean() { // clean texture
		Unbind();
		glDeleteTextures(1, &ID);
	}

	static void Unbind() { // unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	static void Reset() { // reset texture
		glActiveTexture(GL_TEXTURE0);
	}

	const char *Path() { // return path
		return path;
	}

	static TextureData LoadImageData(const char *path, int channels = 0) { // load image data
		TextureData data;
		data.pixels = stbi_load(path, &data.width, &data.height, &data.components, channels);
		if (!data.pixels) { // check for errors
			std::cerr << "Failed to Load Texture: " << path << std::endl;
			FreeImage(data.pixels);
		}
		return data;
	}

	static void FreeImage(unsigned char *pixels) { // free image pixels
		stbi_image_free(pixels);
	}

private:
	TextureData data;
	unsigned int ID;
	const char *path;
};
