#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation {
public:
	Transformation(glm::mat4 model = glm::mat4(1.f)) : model(model) {

	}

	glm::mat4 &GetTransformation() {
		return model;
	}

	void Rotate(float angle, float x, float y, float z, bool radians = false) {
		if (!radians)
			angle = glm::radians(angle);
		model = glm::rotate(model, angle, glm::vec3(x, y, z));
	}

	void Scale(float x, float y, float z) {
		model = glm::scale(model, glm::vec3(x, y, z));
	}

	void Translate(float x, float y, float z) {
		model = glm::translate(model, glm::vec3(x, y, z));
	}

private:
	glm::mat4 model;
};
