#pragma once

#include "Transformation.h"
#include "../graphics/StaticModel.h"
#include "../graphics/Shader.h"
#include "../graphics/StaticModel.h"

class Entity {
public:
	Entity(const char *path, bool gamma = false) : model(path, gamma), shader("src/shaders/model.vs", "src/shaders/model.fs") {

	}

	void SetTransformation(Transformation transformation) {
		this->transformation = transformation;
	}

	Transformation& GetTransformation() {
		return transformation;
	}

	void Render() {
		model.Render(shader, transformation.GetTransformation());
	}

	void Clean() {
		model.Clean();
		shader.Clean();
	}

private:
	StaticModel model;
	Shader shader;
	Transformation transformation;
};
