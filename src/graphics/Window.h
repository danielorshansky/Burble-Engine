#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"

// set glfw input values
#define MOUSE_BUTTON_1 0
#define MOUSE_BUTTON_2 1
#define MOUSE_BUTTON_3 2
#define MOUSE_BUTTON_4 3
#define MOUSE_BUTTON_5 4
#define MOUSE_BUTTON_6 5
#define MOUSE_BUTTON_7 6
#define MOUSE_BUTTON_8 7
#define KEY_ESCAPE 256
#define KEY_ENTER 257
#define KEY_TAB 258
#define KEY_BACKSPACE 259
#define KEY_INSERT 260
#define KEY_DELETE 261
#define KEY_RIGHT 262
#define KEY_LEFT 263
#define KEY_DOWN 264
#define KEY_UP 265
#define KEY_PAGE_UP 266
#define KEY_PAGE_DOWN 267
#define KEY_HOME 268
#define KEY_END 269
#define KEY_CAPS_LOCK 280
#define KEY_SCROLL_LOCK 281
#define KEY_NUM_LOCK 282
#define KEY_PRINT_SCREEN 283
#define KEY_PAUSE 284
#define KEY_F1 290
#define KEY_F2 291
#define KEY_F3 292
#define KEY_F4 293
#define KEY_F5 294
#define KEY_F6 295
#define KEY_F7 296
#define KEY_F8 297
#define KEY_F9 298
#define KEY_F10 299
#define KEY_F11 300
#define KEY_F12 301
#define KEY_F13 302
#define KEY_F14 303
#define KEY_F15 304
#define KEY_F16 305
#define KEY_F17 306
#define KEY_F18 307
#define KEY_F19 308
#define KEY_F20 309
#define KEY_F21 310
#define KEY_F22 311
#define KEY_F23 312
#define KEY_F24 313
#define KEY_F25 314
#define KEY_KP_0 320
#define KEY_KP_1 321
#define KEY_KP_2 322
#define KEY_KP_3 323
#define KEY_KP_4 324
#define KEY_KP_5 325
#define KEY_KP_6 326
#define KEY_KP_7 327
#define KEY_KP_8 328
#define KEY_KP_9 329
#define KEY_KP_DECIMAL 330
#define KEY_KP_DIVIDE 331
#define KEY_KP_MULTIPLY 332
#define KEY_KP_SUBTRACT 333
#define KEY_KP_ADD 334
#define KEY_KP_ENTER 335
#define KEY_KP_EQUAL 336
#define KEY_LEFT_SHIFT 340
#define KEY_LEFT_CONTROL 341
#define KEY_LEFT_ALT 342
#define KEY_LEFT_SUPER 343
#define KEY_RIGHT_SHIFT 344
#define KEY_RIGHT_CONTROL 345
#define KEY_RIGHT_ALT 346
#define KEY_RIGHT_SUPER 347
#define KEY_MENU 348
#define KEY_SPACE 32
#define KEY_APOSTROPHE 39
#define KEY_COMMA 44
#define KEY_MINUS 45
#define KEY_PERIOD 46
#define KEY_SLASH 47
#define KEY_0 48
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57
#define KEY_SEMICOLON 59
#define KEY_EQUAL 61
#define KEY_A 65
#define KEY_B 66
#define KEY_C 67
#define KEY_D 68
#define KEY_E 69
#define KEY_F 70
#define KEY_G 71
#define KEY_H 72
#define KEY_I 73
#define KEY_J 74
#define KEY_K 75
#define KEY_L 76
#define KEY_M 77
#define KEY_N 78
#define KEY_O 79
#define KEY_P 80
#define KEY_Q 81
#define KEY_R 82
#define KEY_S 83
#define KEY_T 84
#define KEY_U 85
#define KEY_V 86
#define KEY_W 87
#define KEY_X 88
#define KEY_Y 89
#define KEY_Z 90
#define KEY_LEFT_BRACKET 91
#define KEY_BACKSLASH 92
#define KEY_RIGHT_BRACKET 93
#define KEY_GRAVE_ACCENT 96

class Window {
public:
	Window(int width, int height, const char *title, bool vsync, bool depth, bool culling) : width(width), height(height) {
		if (!glfwInit()) { // initialize glfw and check for errors
			std::cerr << "Failed to Initialize GLFW" << std::endl;
			exit(1);
		}

		// set window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_VISIBLE, 0);
		glfwWindowHint(GLFW_RESIZABLE, 0);

		window = glfwCreateWindow(width, height, title, 0, 0);
		if (window == 0) { // check for errors
			std::cerr << "Failed to Create Window" << std::endl;
			Clean(); // clean glfw
			exit(1);
		}

		const GLFWvidmode *video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(window, (video_mode->width - width) / 2, (video_mode->height - height) / 2); // center window

		glfwMakeContextCurrent(window);

		if (vsync) // enable vsync
			glfwSwapInterval(1);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { // load glad and check for errors
			std::cerr << "Failed to Initialize glad" << std::endl;
			Clean();
			exit(1);
		}

		if (depth) { // enable depth
			glEnable(GL_DEPTH_TEST);
			fields = fields | GL_DEPTH_BUFFER_BIT;
		}

		glEnable(GL_BLEND); // enable blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (culling) { // enable back face culling
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		glfwShowWindow(window); // show window
	}

	void Clear() { // clear window
		glClear(fields);
	}

	void static SetColor(float r, float g, float b, float a = 1) { // set window clear color
		glClearColor(r, g, b, a);
	}

	void Update() { // update window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	bool ShouldClose() { // check if close attempted
		return glfwWindowShouldClose(window);
	}

	double static GetTime() { // return glfw time
		return glfwGetTime();
	}

	void SetIcon(const char *path) { // set window icon
		TextureData data = Texture::LoadImageData(path, 4);
		GLFWimage images[1];
		images[0].pixels = data.pixels;
		images[0].width = data.width;
		images[0].height = data.height;
		glfwSetWindowIcon(window, 1, images);
		Texture::FreeImage(data.pixels);
	}

	bool KeyDown(int id) { // check if key is down
		return glfwGetKey(window, id);
	}

	bool MouseDown(int mouse_button) { // check if mouse button pressed
		return glfwGetMouseButton(window, mouse_button);
	}

	void Clean() { // clean window
		glfwDestroyWindow(window);
	}

	static void Terminate() { // terminate glfw
		glfwTerminate();
	}

	float AspectRatio() { // return aspect ratio
		return width / height;
	}

private:
	int width, height;
	GLFWwindow *window;
	GLbitfield fields = GL_COLOR_BUFFER_BIT;
};
