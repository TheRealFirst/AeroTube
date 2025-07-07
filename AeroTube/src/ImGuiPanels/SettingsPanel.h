#pragma once

#include "Engine.h"

class SettingsPanel
{
public:
	SettingsPanel() = default;
	SettingsPanel(Engine::Ref<Engine::Camera> camera);

	void Show() { m_IsShowing = true; }

	~SettingsPanel() = default;

	void OnImGuiRender();

private:
	void SaveSettings();
	void LoadSettings();

	// UI helper methods
	void DrawCameraSensitivityUI();
	void DrawCameraSpeedUI();

private:
	bool m_IsShowing;

	// Camera Settings
	Engine::Ref<Engine::Camera> m_Camera;
	float m_CameraSensitivity = 50.0f;
	float m_CameraSpeed = 1.0f;

	// UI state
	bool m_ShowSaveMessage = false;
	float m_SaveMessageTimer = 0.0f;

	// Settings File
	const char* m_Filepath = "./Settings.at"; // Use relative path to current working directory
};