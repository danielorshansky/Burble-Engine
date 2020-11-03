#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

struct TextureData {
	unsigned char *pixels;
	int width, height;
};

class Texture {
public:
	static TextureData LoadImageData(const char *path) {
		TextureData data;
		data.pixels = stbi_load(path, &data.width, &data.height, 0, 4);
		return data;
	}

	static void FreeImage(unsigned char *pixels) {
		stbi_image_free(pixels);
	}
};
