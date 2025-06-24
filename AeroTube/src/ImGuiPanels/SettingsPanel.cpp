#include "SettingsPanel.h"

#include <ImGui\imgui.h>

SettingsPanel::SettingsPanel(Engine::Ref<Engine::Camera> camera)
{
	m_Camera = camera;

	// Try to load settings from file
	LoadSettings();

	// Apply settings to camera
	m_Camera->SetCameraSensitivity(m_CameraSensitivity);
	m_Camera->SetCameraSpeed(m_CameraSpeed);
}


void SettingsPanel::OnImGuiRender()
{
	ImGui::Begin("Settings");


	// Show save confirmation message
	if (m_ShowSaveMessage)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Settings saved!");

		// Decrease timer
		m_SaveMessageTimer -= ImGui::GetIO().DeltaTime;
		if (m_SaveMessageTimer <= 0.0f)
		{
			m_ShowSaveMessage = false;
		}
	}

	if (ImGui::CollapsingHeader("Camera Settings"))
	{
		ImGui::SeparatorText("Sensitivity: ");
		DrawCameraSensitivityUI();

		ImGui::SeparatorText("Speed: ");
		DrawCameraSpeedUI();
	}

	ImGui::End();
}

void SettingsPanel::SaveSettings()
{
	try
	{
		// Open the file with explicit path
		std::ofstream file(m_Filepath);

		// Check if file is open
		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << m_Filepath << std::endl;
			return;
		}

		// Write settings to file
		file << "CameraSensitivity=" << m_CameraSensitivity << std::endl;
		file << "CameraSpeed=" << m_CameraSpeed << std::endl;

		// Close the file
		file.close();

		// Show success message in UI
		m_ShowSaveMessage = true;
		m_SaveMessageTimer = 3.0f; // Show message for 3 seconds

		std::cout << "Settings saved to: " << m_Filepath << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error saving settings: " << e.what() << std::endl;
	}
}

void SettingsPanel::LoadSettings()
{
	try
	{
		// Try to open the settings file
		std::ifstream file(m_Filepath);

		// If file doesn't exist, use default values
		if (!file.is_open())
		{
			std::cout << "Settings file not found, using defaults." << std::endl;
			return;
		}

		// Read settings line by line
		std::string line;
		while (std::getline(file, line))
		{
			// Parse each line as key=value
			size_t equalsPos = line.find('=');
			if (equalsPos != std::string::npos)
			{
				std::string key = line.substr(0, equalsPos);
				std::string value = line.substr(equalsPos + 1);

				// Apply the setting
				if (key == "CameraSensitivity")
				{
					m_CameraSensitivity = std::stof(value);
				}
				else if (key == "CameraSpeed")
				{
					m_CameraSpeed = std::stof(value);
				}
			}
		}

		// Close the file
		file.close();

		std::cout << "Settings loaded from: " << m_Filepath << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error loading settings: " << e.what() << std::endl;
	}
}

void SettingsPanel::DrawCameraSensitivityUI()
{
	ImGui::DragFloat("Camera Sensitivity", &m_CameraSensitivity, 0.5f, 0.0f, 100.0f);

	

	// Add preset buttons for quick sensitivity adjustment
	ImGui::Text("Presets:");
	ImGui::SameLine();
	if (ImGui::Button("Low"))
	{
		m_CameraSensitivity = 20.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button("Medium"))
	{
		m_CameraSensitivity = 50.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button("High"))
	{
		m_CameraSensitivity = 100.0f;
	}

	// Add reset button
	if (ImGui::Button("Reset Sensitivity to Default"))
	{
		m_CameraSensitivity = 50.0f;
	}

	if (m_Camera->GetCameraSensitivity() != m_CameraSensitivity)
	{
		m_Camera->SetCameraSensitivity(m_CameraSensitivity);
		SaveSettings();
	}
}

void SettingsPanel::DrawCameraSpeedUI()
{
	ImGui::DragFloat("Camera Speed", &m_CameraSpeed, 0.1f, 0.0f, 10.0f);

	// Add reset button
	if (ImGui::Button("Reset Speed to Default"))
	{
		m_CameraSpeed = 1.0f;
	}

	if (m_Camera->GetCameraSpeed() != m_CameraSpeed)
	{
		m_Camera->SetCameraSpeed(m_CameraSpeed);
		SaveSettings();
	}
}
