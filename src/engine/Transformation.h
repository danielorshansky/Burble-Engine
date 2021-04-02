#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation {
public:
	Transformation(glm::mat4 model = glm::mat4(1.f)) : model(model) { // initialize model matrix

	}

	glm::mat4 &GetTransformation() { // return model matrix
		return model;
	}

	void Rotate(float angle, float x, float y, float z, bool radians = false) { // rotate
		if (!radians)
			angle = glm::radians(angle);
		model = glm::rotate(model, angle, glm::vec3(x, y, z));
	}

	void Scale(float x, float y, float z) { // scale
		model = glm::scale(model, glm::vec3(x, y, z));
	}

	void Translate(float x, float y, float z) { // translate
		model = glm::translate(model, glm::vec3(x, y, z));
	}

private:
	glm::mat4 model;
};
