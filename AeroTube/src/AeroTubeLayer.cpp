#include "AeroTubeLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {
	AeroTubeLayer::AeroTubeLayer() : Layer("AeroTube")
	{
	}

	void AeroTubeLayer::OnAttach()
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 3840;
		fbSpec.Height = 2160;
		m_Renderer = new Renderer(fbSpec.Width, fbSpec.Height);
		m_Framebuffer = CreateRef<Framebuffer>(fbSpec);

		m_Camera = Camera(fbSpec.Width, fbSpec.Height, glm::vec3(0, 0, -2), 45, 0.1f, 1000.0f);

		m_SettingsPanel = SettingsPanel(&m_Camera);
	}

	void AeroTubeLayer::OnDetach()
	{
		delete m_Renderer;
	}

	void AeroTubeLayer::OnUpdate(Timestep ts)
	{
		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera.Resize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_Camera.OnUpdate(ts);

		m_Framebuffer->Bind();

		m_Renderer->Clear({ 0.1f, 0.1f, 0.1f, 1 });

		m_Framebuffer->ClearAttachment(1, -1);

		m_Renderer->Render(ts, m_Camera);

		m_Framebuffer->Unbind();
	}

	void AeroTubeLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 370.0f;
		float minWinSizeX = style.WindowMinSize.x;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{

				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{

				}

				if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
				{

				}

				if (ImGui::MenuItem("Preferences"))
				{
					// float fontsize;
					//
					// // Options menu
					// if (ImGui::Begin("Preferences"))
					// {
					// 	ImGui::DragFloat("Set Font Size", &fontsize);
					// 	ImGui::End();
					// }
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit", "Ctrl+W")) CloseApplication();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Models"))
			{
				if (ImGui::MenuItem("Bunny"))
				{
					m_Renderer->LoadModel("Assets/Models/bunny/scene.gltf");

					// Apply specific transformations for the bunny model
					m_Renderer->SetModelPosition(glm::vec3(0.0f, 0.0f, 0.0f));
					m_Renderer->SetModelRotation(glm::vec3(0.0f, 0.0f, 0.0f));
					m_Renderer->SetModelScale(glm::vec3(1.0f));
				}
				if (ImGui::MenuItem("Adam Head"))
				{
					m_Renderer->LoadModel("Assets/Models/adamHead/adamHead.gltf");

					// Apply specific transformations for the adamHead model
					m_Renderer->SetModelPosition(glm::vec3(0.0f, -0.5f, 0.0f));
					m_Renderer->SetModelRotation(glm::vec3(0.0f, 180.0f, 0.0f));
					m_Renderer->SetModelScale(glm::vec3(0.5f));

					// Set optimal lighting for Adam Head
					m_Renderer->SetLightPosition(glm::vec3(0.0f, 1.0f, 2.0f));
					m_Renderer->SetLightColor(glm::vec4(1.8f, 1.8f, 1.8f, 1.0f));
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Stats");

		ImGui::Text("No Stats available yet");

		ImGui::End();

		// Model Settings Panel
		ImGui::Begin("Model Settings");

		// Get current model transformations
		glm::vec3 modelPosition = m_Renderer->GetModelPosition();
		glm::vec3 modelRotation = m_Renderer->GetModelRotation();
		glm::vec3 modelScale = m_Renderer->GetModelScale();

		// Get current light properties
		glm::vec3 lightPosition = m_Renderer->GetLightPosition();
		glm::vec4 lightColor = m_Renderer->GetLightColor();

		// Position controls
		ImGui::Text("Position");
		if (ImGui::DragFloat3("##Position", glm::value_ptr(modelPosition), 0.1f))
		{
			m_Renderer->SetModelPosition(modelPosition);
		}

		// Rotation controls
		ImGui::Text("Rotation");
		if (ImGui::DragFloat3("##Rotation", glm::value_ptr(modelRotation), 1.0f))
		{
			m_Renderer->SetModelRotation(modelRotation);
		}

		// Scale controls
		ImGui::Text("Scale");
		if (ImGui::DragFloat3("##Scale", glm::value_ptr(modelScale), 0.1f, 0.1f, 10.0f))
		{
			m_Renderer->SetModelScale(modelScale);
		}

		ImGui::Separator();
		ImGui::Text("Light Settings");

		// Light position controls
		ImGui::Text("Light Position");
		if (ImGui::DragFloat3("##LightPosition", glm::value_ptr(lightPosition), 0.1f))
		{
			m_Renderer->SetLightPosition(lightPosition);
		}

		// Light color controls
		ImGui::Text("Light Color");
		if (ImGui::ColorEdit3("##LightColor", glm::value_ptr(lightColor)))
		{
			m_Renderer->SetLightColor(lightColor);
		}

		// Presets
		ImGui::Text("Presets");

		// Reset button
		if (ImGui::Button("Reset"))
		{
			m_Renderer->SetModelPosition(glm::vec3(0.0f));
			m_Renderer->SetModelRotation(glm::vec3(0.0f));
			m_Renderer->SetModelScale(glm::vec3(1.0f));
		}

		ImGui::SameLine();

		// Bunny preset
		if (ImGui::Button("Bunny"))
		{
			m_Renderer->SetModelPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			m_Renderer->SetModelRotation(glm::vec3(0.0f, 0.0f, 0.0f));
			m_Renderer->SetModelScale(glm::vec3(1.0f));
		}

		ImGui::SameLine();

		// Adam Head preset
		if (ImGui::Button("Adam Head"))
		{
			m_Renderer->SetModelPosition(glm::vec3(0.0f, -0.5f, 0.0f));
			m_Renderer->SetModelRotation(glm::vec3(0.0f, 180.0f, 0.0f));
			m_Renderer->SetModelScale(glm::vec3(0.5f));

			// Set optimal lighting for Adam Head
			m_Renderer->SetLightPosition(glm::vec3(0.0f, 1.0f, 2.0f));
			m_Renderer->SetLightColor(glm::vec4(1.8f, 1.8f, 1.8f, 1.0f));
		}

		ImGui::End();

		m_SettingsPanel.OnImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		// Only process camera inputs when viewport is focused or hovered
		bool processInputs = m_ViewportFocused || m_ViewportHovered;
		m_Camera.SetProcessInputs(processInputs);
		Application::Get().GetImGuiLayer()->BlockEvents(!processInputs);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
		ImGui::Image((ImTextureID)(intptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::End();
	}

	void AeroTubeLayer::OnEvent(Event& event)
	{
	}

	bool AeroTubeLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					// NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					// OpenScene();

				break;
			}
			case Key::S:
			{
				if (control && shift)
					// SaveSceneAs();

				break;
			}
			case Key::W:
			{
				if (control)
				{
					CloseApplication();
				}
				break;
			}
			/*
			case Key::Comma:
			{
				if (control)
				{
					ShowSettings();
				}
				break;
			}
			*/
		}
	}

	bool AeroTubeLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	void AeroTubeLayer::CloseApplication()
	{
		Application::Get().Close();
	}

}