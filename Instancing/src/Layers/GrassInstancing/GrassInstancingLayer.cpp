#include "GrassInstancingLayer.h"
#include <GLFW/glfw3.h>
#include "../../Helpers.h"

GrassInstancingLayer::GrassInstancingLayer(Camera* camera) :
	skyColor(0.53f, 0.81f, 0.92f, 1.0f),
	shader("resources/shaders/grassInstancing/vertex.glsl", "resources/shaders/grassInstancing/fragment.glsl"),
	VAO(0), meshVBO(0), instanceVBO(0), displaySkybox(true),
	grassVertices{
		-0.10f, 0.0f, 0.0f,  0.2f, 0.8f, 0.2f,
		 0.10f, 0.0f, 0.0f,  0.2f, 0.8f, 0.2f,
		-0.10f, 0.5f, 0.0f,  0.2f, 0.8f, 0.2f,
		 0.10f, 0.5f, 0.0f,  0.2f, 0.8f, 0.2f,
		-0.05f, 1.0f, 0.0f,  0.2f, 0.8f, 0.2f,
		 0.00f, 1.5f, 0.0f,  0.2f, 0.8f, 0.2f
	}
{
	this->camera = camera;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

GrassInstancingLayer::~GrassInstancingLayer() {

}

void GrassInstancingLayer::OnAttach() {
	std::vector<std::string> faces {
		"resources/skybox/day/right.jpg",
		"resources/skybox/day/left.jpg",
		"resources/skybox/day/top.jpg",
		"resources/skybox/day/bottom.jpg",
		"resources/skybox/day/front.jpg",
		"resources/skybox/day/back.jpg"
	};
	skybox = new Skybox(faces);

	std::vector<GrassInstance> instances(GRASS_INSTANCES);

	std::mt19937 rng(1337);
	std::uniform_real_distribution<float> posDist(-grassSpread, grassSpread);
	std::uniform_real_distribution<float> rotDist(0.0f, 6.2831853f); // 0 to 2*PI
	std::uniform_real_distribution<float> scaleDist(scaleMin, scaleMax);

	for (int i = 0; i < GRASS_INSTANCES; i++)
	{
		instances[i].position = glm::vec2(posDist(rng), posDist(rng));
		instances[i].rotation = rotDist(rng);
		instances[i].scale = scaleDist(rng);
	}

	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GrassInstance) * GRASS_INSTANCES, instances.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &meshVBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferData(GL_ARRAY_BUFFER, grassVertices.size() * sizeof(float), grassVertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GrassInstance), (void*)offsetof(GrassInstance, position));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GrassInstance), (void*)offsetof(GrassInstance, rotation));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(GrassInstance), (void*)offsetof(GrassInstance, scale));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GrassInstancingLayer::OnDetach() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &meshVBO);
	glDeleteBuffers(1, &instanceVBO);

	delete skybox;
}

void GrassInstancingLayer::OnUpdate() {
	glClearColor(skyColor[0], skyColor[1], skyColor[2], skyColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	shader.setFloat("uTime", (float)glfwGetTime());
	
	int width, height;
	glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 100.0f);

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, GRASS_INSTANCES);
	glBindVertexArray(0);

	if (displaySkybox) {
		skybox->Draw(view, projection);
	}
}

void GrassInstancingLayer::OnImGuiRender() {
    ImGui::Begin(GetName().c_str());

    bool grassChanged = false;
    if (ImGui::SliderFloat("Grass Spread", &grassSpread, 10.0f, 500.0f)) grassChanged = true;
    if (ImGui::SliderFloat("Scale Min", &scaleMin, 0.1f, 2.0f)) grassChanged = true;
    if (ImGui::SliderFloat("Scale Max", &scaleMax, 0.1f, 3.0f)) grassChanged = true;

	ImGui::Checkbox("Display Skybox", &displaySkybox);


    if (grassChanged) {
        std::vector<GrassInstance> instances(GRASS_INSTANCES);
        std::mt19937 rng(1337);
        std::uniform_real_distribution<float> posDist(-grassSpread, grassSpread);
        std::uniform_real_distribution<float> rotDist(0.0f, 6.2831853f);
        
        if (scaleMin > scaleMax) scaleMin = scaleMax;
        
        std::uniform_real_distribution<float> scaleDist(scaleMin, scaleMax);

        for (int i = 0; i < GRASS_INSTANCES; i++)
        {
            instances[i].position = glm::vec2(posDist(rng), posDist(rng));
            instances[i].rotation = rotDist(rng);
            instances[i].scale = scaleDist(rng);
        }

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GrassInstance) * GRASS_INSTANCES, instances.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ImGui::ColorEdit4("Sky Color", &skyColor[0]);

    ImGui::End();
}