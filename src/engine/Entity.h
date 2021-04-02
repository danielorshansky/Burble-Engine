#pragma once

#include "Transformation.h"
#include "Camera.h"
#include "../graphics/StaticModel.h"
#include "../graphics/Shader.h"
#include "../graphics/StaticModel.h"
#include "../graphics/Window.h"

class Entity {
public:
	Entity(const char *path, bool gamma = false) : model(path, gamma), shader("src/shaders/model.vs", "src/shaders/model.fs") { // initialize shader and model

	}

	void SetTransformation(Transformation transformation) {
		this->transformation = transformation;
	}

	Transformation &GetTransformation() { // returns transformation
		return transformation;
	}

	void Render(Camera &camera, Window &window) { // render model
		model.Render(shader, transformation.GetTransformation(), camera, window);
	}

	void Clean() { // clean model and shader
		model.Clean();
		shader.Clean();
	}

private:
	StaticModel model;
	Shader shader;
	Transformation transformation;
};
