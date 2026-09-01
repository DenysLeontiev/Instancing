#pragma once

#include "../Layer.h"
#include "GrassInstance.h"
#include "../../Skybox.h"

#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <stb_image.h>

class GrassInstancingLayer : public Layer {
public:
	const int GRASS_INSTANCES = 1'000'000;

	GrassInstancingLayer(Camera* camera);
	~GrassInstancingLayer();

	void OnAttach();
	void OnDetach();
	void OnUpdate();
	void OnImGuiRender();

private:
	Shader shader;
	Camera* camera;

	unsigned int VAO;
	unsigned int meshVBO;
	unsigned int instanceVBO;

	std::vector<float> grassVertices;
	glm::vec4 skyColor;

	float grassSpread = 100.0f;
	float scaleMin = 0.7f;
	float scaleMax = 1.3f;

	Skybox* skybox;
};