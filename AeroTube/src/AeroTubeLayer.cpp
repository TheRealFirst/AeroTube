#include "AeroTubeLayer.h"

#include <imgui/imgui.h>

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

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Stats");

		ImGui::Text("No Stats available yet");

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
		return false;
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