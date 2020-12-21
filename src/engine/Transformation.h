#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation {
public:
	Transformation(glm::mat4 transformation = glm::mat4(1.f)) : transformation(transformation) {

	}

	glm::mat4 &GetTransformation() {
		return transformation;
	}

	void Rotate(float angle, float x, float y, float z) {
		transformation = glm::rotate(transformation, angle, glm::vec3(x, y, z));
	}

	void Scale(float x, float y, float z) {
		transformation = glm::scale(transformation, glm::vec3(x, y, z));
	}

	void Translate(float x, float y, float z) {
		transformation = glm::translate(transformation, glm::vec3(x, y, z));
	}

private:
	glm::mat4 transformation;
};
