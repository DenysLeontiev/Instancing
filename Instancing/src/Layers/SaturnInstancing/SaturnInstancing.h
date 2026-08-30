#pragma once

#include "../Layer.h"

#include "../../Shader.h"
#include "../../Camera.h"

#include <vector>
#include <random>
#include "../../Model.h"

class SaturnInstancing : public Layer {
	const int ASTEROID_INSTANCES = 30'000;
public:

	SaturnInstancing(Camera* camera);
	~SaturnInstancing();

	void OnAttach();
	void OnDetach();
	void OnUpdate();

private:
	Shader planetShader;
	Shader asteroidShader;

	Camera* camera;

	Model planetModel;
	Model asteroidModel;

	glm::vec4 skyColor;
	glm::mat4* modelMatrices;

	unsigned int asteroidsVBO;

	float tiltInDegrees = 26.73f;

	float planetRotMultiplier = 0.1f;
	float asteroidsRotMultiplier = 0.3f;

	float beltHeight = 0.4f;

	float radius = 40.0f;
	float offset = 5.0f;
};