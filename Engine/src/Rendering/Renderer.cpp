#include "atpch.h"

#include "Renderer.h"
#include "glad\glad.h"
#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "imgui.h"
#include <filesystem>


namespace Engine {
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;

	void Renderer::Init()
	{
		glEnable(GL_DEPTH_TEST);

		// When face culling is enabled, use these settings:
		glEnable(GL_CULL_FACE); 
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
	}

	void Renderer::Clear(const glm::vec4& color)
	{
		// Specify the color of the background
		glClearColor(color.r, color.g, color.b, color.a);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Render(uint32_t indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
}
