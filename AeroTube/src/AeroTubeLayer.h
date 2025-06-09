#pragma once
#include <Engine.h>
#include "ImGuiPanels\SettingsPanel.h"

namespace Engine {
	class AeroTubeLayer : public Layer
	{
    public:
        AeroTubeLayer();
        ~AeroTubeLayer() override = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        void CloseApplication();

    private:
        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = { 0, 0 };
        glm::vec2 m_ViewportBounds[2];

        Ref<Framebuffer> m_Framebuffer;

        Camera m_Camera;
        Scene m_Scene;

        SettingsPanel m_SettingsPanel;
	};
}
