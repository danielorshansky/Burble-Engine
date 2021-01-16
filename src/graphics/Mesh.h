#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Shader.h"
#include "../engine/Camera.h"

struct Material {
	std::vector<glm::vec2> tex_coords;
	std::vector<Texture> textures;
	std::vector<glm::vec4> colors;
};

class Mesh {
public:
	Mesh(const std::vector<glm::vec3> &positions, const Material &material, const std::vector<unsigned int> &indices) : material(material), textured(!material.textures.empty()), vertex_count(indices.size()) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(4, VBOs);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (!textured) {
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(material.colors[0]) * material.colors.size(), &material.colors[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(material.tex_coords[0]) * material.tex_coords.size(), &material.tex_coords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Render(Shader &shader, glm::mat4 &model, Camera &camera, Window &window) {
		shader.Bind();
		shader.UniformMat4("model", model);
		glm::mat4 matrix = camera.GetViewMatrix();
		shader.UniformMat4("view", matrix);
		matrix = camera.GetProjectionMatrix(window.AspectRatio());
		shader.UniformMat4("projection", matrix);
		shader.UniformInt("textured", int(textured));
		if (textured) {
			for (unsigned int i = 0; i < material.textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				shader.UniformInt(("sampler" + std::to_string(i)).c_str(), i);
				material.textures[i].Bind();
			}
		}
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		if (textured) {
			Texture::Unbind();
			Texture::Reset();
		}
		Shader::Unbind();
	}

	void Clean() {
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		for (unsigned int &VBO : VBOs)
			glDeleteBuffers(1, &VBO);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
	}
	
private:
	unsigned int VAO, VBOs[4], vertex_count;
	Material material;
	bool textured;
};
