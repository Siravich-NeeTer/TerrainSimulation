#include <iostream>
#include <string>

#include "Window.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Mesh.h"
#include "Camera.h"

#include "Terrain.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Screen Size
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

bool isCameraMove = false;
glm::vec3 lightPos = glm::vec3(5.0f);

void UpdateUIMenu(Terrain& terrain);

int main()
{
	Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "Cloth_Simulation");
	window.Init();

	if (window.IsNull())
	{
		std::cerr << "Error(WINDOW_NULL) : Cannot open window \n";
		return -1;
	}

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	Camera cam(glm::vec3(0,0,8));
	Shader cubeShader("CubeShader");
	cubeShader.AttachShader(BaseShader("Shader/object.vert"));
	cubeShader.AttachShader(BaseShader("Shader/object.frag"));
	cubeShader.LinkProgram();

	Mesh cubeMesh;
	Terrain terrain(100, 100);

	// --------------- Game Loop ---------------
	float prevTime = 0.0f;
	float dt = 0.0f;
	while (!window.ShouldClose())
	{
		float currentTime = glfwGetTime();
		dt = currentTime - prevTime;
		window.PollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Update Camera
		if (isCameraMove)
			cam.ProcessMousesMovement();
		cam.Input(dt);

		GLint polygonMode;
		glGetIntegerv(GL_POLYGON_MODE, &polygonMode);

		if (Input::isKeyBeginPressed(GLFW_KEY_LEFT_ALT))
		{
			cam.ResetMousePosition();
			isCameraMove = !isCameraMove;
			if (isCameraMove)
				glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			else
				glfwSetInputMode(window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else if (Input::isKeyBeginPressed(GLFW_KEY_SPACE))
		{
			glPolygonMode(GL_FRONT_AND_BACK, polygonMode == GL_LINE ? GL_FILL : GL_LINE);
		}

		UpdateUIMenu(terrain);

		// -------------------------- Render Sun --------------------------
		cubeShader.Activate();
		cubeShader.SetVec3("u_Color", glm::vec3(1.0f, 0.647f, 0.0f));
		cubeShader.SetMat4("u_Model", glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
		cubeShader.SetMat4("u_View", cam.GetViewMatrix());
		cubeShader.SetMat4("u_Projection", glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f));

		cubeMesh.Draw(GL_TRIANGLES);
		// ----------------------------------------------------------------

		// -------------------------- Render Terrain --------------------------
		const Shader& terrainShader = terrain.GetShader();
		terrainShader.Activate();
		terrainShader.SetBool("u_IsWireFrame", polygonMode == GL_LINE ? true : false);
		terrainShader.SetFloat("u_Time", currentTime);
		terrainShader.SetVec3("u_LightPos", lightPos);
		terrainShader.SetVec3("u_ViewPos", cam.GetPosition());
		terrainShader.SetMat4("u_Model", glm::mat4(1.0f));
		terrainShader.SetMat4("u_View", cam.GetViewMatrix());
		terrainShader.SetMat4("u_Projection", glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f));

		terrain.Render(terrainShader);
		// --------------------------------------------------------------------

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Post - Render
		Input::endFrame();
		window.SwapBuffers();

		prevTime = currentTime;
	}

	// --------------- End Program ---------------
	// Clean Application & Window
	window.Destroy();
}

void UpdateUIMenu(Terrain& terrain)
{
	ImGui::Begin("Terrain Simulation");
	
	if (ImGui::CollapsingHeader("Terrain"))
	{
		ImGui::SliderFloat("Amplitude", &terrain.GetAmplitude(), 0.1f, 10.0f);
		ImGui::SliderInt("Octave", &terrain.GetOctave(), 1, 16);
		ImGui::SliderFloat("Persistance", &terrain.GetPersistance(), 0.1f, 1.0f);
		ImGui::SliderFloat("Lunarcity", &terrain.GetLunarcity(), 0.1f, 5.0f);
		ImGui::Checkbox("Smooth", &terrain.IsSmooth());
		ImGui::SliderInt("Seed", &terrain.GetSeed(), 1, SHRT_MAX);
		ImGui::SliderFloat("Delta", &terrain.GetDelta(), 0.001f, 0.1f);
	}
	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::ColorPicker3("Light Color", &terrain.GetLightColor()[0]);
	}
	if (ImGui::CollapsingHeader("Fog"))
	{
		ImGui::Checkbox("Fog Active", &terrain.GetFog().IsActive());
		ImGui::SliderFloat("Min Distance", &terrain.GetFog().GetMinDistance(), 0.1f, terrain.GetFog().GetMaxDistance());
		ImGui::SliderFloat("Max Distance", &terrain.GetFog().GetMaxDistance(), terrain.GetFog().GetMinDistance(), 20.0f);
		ImGui::SliderFloat("Density", &terrain.GetFog().GetDensity(), 0.1f, 10.0f);
		ImGui::SliderFloat("Gradient", &terrain.GetFog().GetGradient(), 0.1f, 10.0f);
		ImGui::ColorPicker3("Fog Color", &terrain.GetFog().GetColor()[0]);
	}
	
	ImGui::End();
}