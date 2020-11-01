#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"

class Window {
public:
	Window(int width, int height, const char* title, bool vsync, bool depth) : width(width), height(height) {
		if (!glfwInit()) {
			std::cerr << "Failed to Initialize GLFW" << std::endl;
			exit(1);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, 0);
		glfwWindowHint(GLFW_RESIZABLE, 0);

		window = glfwCreateWindow(width, height, title, 0, 0);
		if (window == 0) {
			std::cerr << "Failed to Create Window" << std::endl;
			Clean();
			exit(1);
		}

		const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(window, (video_mode->width - width) / 2, (video_mode->height - height) / 2);

		glfwMakeContextCurrent(window);

		if (vsync)
			glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to Initialize glad" << std::endl;
			Clean();
			exit(1);
		}

		if (depth) {
			glEnable(GL_DEPTH_TEST);
			fields = fields | GL_DEPTH_BUFFER_BIT;
		}

		glfwShowWindow(window);
	}

	void Clear() {
		glClear(fields);
	}

	void SetColor(float r, float g, float b, float a = 1) {
		glClearColor(r, g, b, a);
	}

	void Update() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	bool ShouldClose() {
		return glfwWindowShouldClose(window);
	}

	double GetTime() {
		return glfwGetTime();
	}

	void SetIcon(const char* path) {
		Image image = Texture::LoadImageData(path);
		GLFWimage images[1];
		images[0].pixels = image.pixels;
		images[0].width = image.width;
		images[0].height = image.height;
		glfwSetWindowIcon(window, 1, images);
		Texture::FreeImage(image.pixels);
	}

	void Clean() {
		glfwTerminate();
	}

private:
	int width, height;
	GLFWwindow* window;

	GLbitfield fields = GL_COLOR_BUFFER_BIT;
};
