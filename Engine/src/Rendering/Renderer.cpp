#include "atpch.h"



#include "Renderer.h"
#include "glad\glad.h"
#include "stb_image.h"



Renderer::Renderer(uint32_t width, uint32_t height, GLFWwindow* window) : shaderProgram("Assets/Shaders/default.vert", "Assets/Shaders/default.frag"), m_Width(width), m_Height(height) , m_Window(window),
m_Camera(m_Width, m_Height, glm::vec3(0.0f, 0.0f, 2.0f)), model("Assets/Models/bunny/scene.gltf") 
{	
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);


	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);
	/*
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	*/

	m_TestScene = new Scene();

	// Make sure window is valid
	if (!m_Window) {
		LOG_FATAL("Failed creating the window");
	}

	// Initialize ImGui layer
	m_ImGuiLayer = std::make_unique<ImGuiLayer>();
	m_ImGuiLayer->InitializeImGui(m_Window);
}

Renderer::~Renderer()
{
	shaderProgram.Delete();
	delete m_TestScene;
	
}

void Renderer::Render(float deltaTime)
{
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Tell OpenGL which Shader Program we want to use

	m_ImGuiLayer->ImGuiNewFrame();

	m_Camera.Inputs(m_Window, deltaTime);
	m_Camera.updateMatrix(45.0f, 0.1f, 100.0f);
	

	model.Draw(shaderProgram, m_Camera);

	m_TestScene->DrawScene(m_Camera.Position, m_Camera.Orientation, m_Camera.Up, m_Width, m_Height);
	
	m_ImGuiLayer->ImGuiRender();


}

void Renderer::Resize(uint32_t newWidth, uint32_t newHeight)
{
	m_Width = newWidth;
	m_Height = newHeight;

	m_Camera.Resize(m_Width, m_Height);
}
