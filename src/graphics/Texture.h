#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

struct Image {
	unsigned char* pixels;
	int width, height;
};

class Texture {
public:
	static Image LoadImageData(const char* path) {
		Image image;
		image.pixels = stbi_load(path, &image.width, &image.height, 0, 4);
		return image;
	}

	static void FreeImage(unsigned char* pixels) {
		stbi_image_free(pixels);
	}
};
