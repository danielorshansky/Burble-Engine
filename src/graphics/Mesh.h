#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Shader.h"
#include "../engine/Camera.h"

struct Material { // material for mesh
	std::vector<glm::vec2> tex_coords;
	std::vector<Texture> textures;
	std::vector<glm::vec4> colors;
};

class Mesh {
public:
	Mesh(const std::vector<glm::vec3> &positions, const Material &material, const std::vector<unsigned int> &indices) : material(material), textured(!material.textures.empty()), vertex_count(indices.size()) {
		// initialize VAOs and VBOs
		glGenVertexArrays(1, &VAO);
		glGenBuffers(4, VBOs);
		glBindVertexArray(VAO);

		// set position VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// set texture/color VBO
		if (!textured) { // if not textured set color
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(material.colors[0]) * material.colors.size(), &material.colors[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}
		else { // if textured set texture
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(material.tex_coords[0]) * material.tex_coords.size(), &material.tex_coords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// set indices VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOs[3]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

		// unbind VAO
		glBindVertexArray(0);
	}

	void Render(Shader &shader, glm::mat4 &model, Camera &camera, Window &window) { // render mesh
		shader.Bind(); // bind shader
		shader.UniformMat4("model", model); // set uniform model matrix
		glm::mat4 matrix = camera.GetViewMatrix();
		shader.UniformMat4("view", matrix); // set uniform view matrix
		matrix = camera.GetProjectionMatrix(window.AspectRatio());
		shader.UniformMat4("projection", matrix); // set uniform projection matrix
		shader.UniformInt("textured", int(textured)); // set uniform textured bool
		if (textured) { // if textured bind all textures
			for (unsigned int i = 0; i < material.textures.size(); i++) { // allow for multiple textures. note: if multiple textures are used, shaders will have to be edited to fit number of textures
				glActiveTexture(GL_TEXTURE0 + i);
				shader.UniformInt(("sampler" + std::to_string(i)).c_str(), i);
				material.textures[i].Bind();
			}
		}
		glBindVertexArray(VAO); // bind VAO
		glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, 0); // render
		glBindVertexArray(0); // unbind VAO
		if (textured) { // clean texture if textured
			Texture::Unbind();
			Texture::Reset();
		}
		Shader::Unbind(); // unbind shader
	}

	void Clean() { // clean VAOs and VBOs
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
