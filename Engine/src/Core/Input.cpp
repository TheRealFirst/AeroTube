﻿#include "atpch.h"

#include "Input.h"

#include "Application.h"
#include <GLFW/glfw3.h>

namespace Engine
{
    bool Input::IsKeyPressed(const KeyCode key)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetWindow());

        auto state = glfwGetKey(window, static_cast<uint32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(const MouseCode button)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetWindow());

        auto state = glfwGetMouseButton(window, static_cast<uint32_t>(button));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::GetMousePosition()
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetWindow());

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }

    float Input::GetMouseX()
    {
        return  GetMousePosition().x;
    }

    float Input::GetMouseY()
    {
        return  GetMousePosition().y;
    }
}