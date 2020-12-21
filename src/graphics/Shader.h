#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader(const char *vertex_path, const char *fragment_path) {
		std::string vertex_src_str, fragment_src_str;
		std::ifstream vertex_file, fragment_file;
		vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vertex_file.open(vertex_path);
			fragment_file.open(fragment_path);
			std::stringstream vertex_stream, fragment_stream;
			vertex_stream << vertex_file.rdbuf();
			fragment_stream << fragment_file.rdbuf();
			vertex_file.close();
			fragment_file.close();
			vertex_src_str = vertex_stream.str();
			fragment_src_str = fragment_stream.str();
		}
		catch (std::ifstream::failure &exception) {
			std::cerr << "Failed to Load Shader File" << std::endl;
		}
		const char *vertex_src = vertex_src_str.c_str(), *fragment_src = fragment_src_str.c_str();

		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertex_src, 0);
		glCompileShader(vertex);
		CompileErrors(vertex, false);
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragment_src, 0);
		glCompileShader(fragment);
		CompileErrors(fragment, false);

		id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);
		glValidateProgram(id);
		CompileErrors(id, true);
		glDetachShader(id, vertex);
		glDetachShader(id, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Bind() {
		glUseProgram(id);
	}

	static void Unbind() {
		glUseProgram(0);
	}

	void Clean() {
		Unbind();
		glDeleteProgram(id);
	}

	void UniformInt(const char *name, int value) {
		glUniform1i(glGetUniformLocation(id, name), value);
	}

	void UniformMat4(const char *name, glm::mat4 &matrix) {
		glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

private:
	unsigned int id;

	void CompileErrors(unsigned int shader, bool program) {
		int success;
		char info_log[1024];
		if (!program) {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, 0, info_log);
				std::cerr << "Shader Compilation Error: " << info_log << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, 0, info_log);
				std::cerr << "Program Linking Error: " << info_log << std::endl;
			}
			glGetProgramiv(shader, GL_VALIDATE_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, 0, info_log);
				std::cerr << "Program Validation Warning: " << info_log << std::endl;
			}
		}
	}
};
