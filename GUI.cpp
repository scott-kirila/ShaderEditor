//
// Created by Scott Kirila on 2024/04/29.
//

#include "GUI.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Window.h"

GUI::GUI(Window& window, float viewWidth, float viewHeight, const char* glslVersion) : m_FramebufferSize(viewWidth, viewHeight) {
    // Set up Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.Fonts->AddFontFromFileTTF("../OpenSans-Regular.ttf", 32.0f);

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        style.Colors[ImGuiCol_FrameBg] = ImVec4{ 0.1, 0.1, 0.1, 1.0 };
        style.Colors[ImGuiCol_Button] = ImVec4{ 0.2, 0.2, 0.2, 1.0 };
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.15, 0.0, 0.25, 1.0 };
        style.Colors[ImGuiCol_ButtonActive] = ImVec4{ 0.10, 0.0, 0.2, 1.0 };
        style.Colors[ImGuiCol_TabActive] = ImVec4{ 0.15, 0.15, 0.15, 1.0 };
        style.Colors[ImGuiCol_TabHovered] = ImVec4{ 0.2, 0.2, 0.2, 1.0 };
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.1, 0.1, 0.1, 1.0 };
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.0, 0.0, 0.0, 1.0 };

        style.FrameRounding = 5.0f;
        style.TabRounding = 5.0f;
    }

    // Set up backends
    ImGui_ImplGlfw_InitForOpenGL(window.m_Window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
