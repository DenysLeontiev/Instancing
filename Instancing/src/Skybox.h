#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

class Skybox {
public:
	Skybox(const std::vector<std::string>& faces);
	~Skybox();

	void Draw(const glm::mat4& view, const glm::mat4 projection);
private:
	Shader shader;

	unsigned int skyboxVAO, skyboxVBO;
	unsigned int cubemapTexture;
};