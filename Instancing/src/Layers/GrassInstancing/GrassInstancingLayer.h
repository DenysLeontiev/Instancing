#pragma once

#include "../Layer.h"
#include "../../Shader.h"
#include "../../Camera.h"
#include "GrassInstance.h"

#include <vector>
#include <random>

class GrassInstancingLayer : public Layer {
public:
	const int GRASS_INSTANCES = 1'000'000;

	GrassInstancingLayer(Camera* camera);
	~GrassInstancingLayer();

	void OnAttach();
	void OnDetach();
	void OnUpdate();

private:
	Shader shader;
	Camera* camera;

	unsigned int VAO;
	unsigned int meshVBO;
	unsigned int instanceVBO;

	std::vector<float> grassVertices;
	glm::vec4 skyColor;
};