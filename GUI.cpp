//
// Created by Scott Kirila on 2024/04/29.
//

#include <iostream>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include "Callbacks.h"
#include "GUI.h"

#include <Windows.h>

#include "RenderingOpenGL.h"
#include "Shader.h"
#include "TextCompletion.h"
#include "Window.h"

namespace ImGui {
    bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags,
                               const ImGuiInputTextCallback callback, void* user_data)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;
        flags |= ImGuiInputTextFlags_CallbackAlways;
        flags |= ImGuiInputTextFlags_CallbackCharFilter;
        // flags |= ImGuiInputTextFlags_CallbackCompletion;
        flags |= ImGuiInputTextFlags_CallbackEdit;
        // flags |= ImGuiInputTextFlags_CallbackHistory;

        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;

        return InputTextMultiline(label, const_cast<char *>(str->c_str()), str->capacity() + 1, size, flags, callback, &cb_user_data);
    }
}

GUI::GUI(Window* window, Rendering::OpenGL* renderer, const float viewWidth, const float viewHeight, const char* glslVersion)
    : m_GlslVersion(glslVersion), m_FramebufferSize(viewWidth, viewHeight),
      m_Window(window), m_Renderer(renderer) {
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

    m_IO = &io;

    // Set up backends
    ImGui_ImplGlfw_InitForOpenGL(window->m_Window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

GUI::~GUI() {
    std::cout << "GUI\n";
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Loop() {
    m_Window->BeginLoop([this]() {
        LoopSetup();
        LoopBody();
        LoopTeardown();
    });
}

void GUI::LoopSetup() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (m_ShowDemoWindow)
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);

    // Dockspace
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    const ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
}

void GUI::LoopBody() const {
    {
        ShowMenuBar();

        static std::string fragmentShaderString = m_GlslVersion + std::string("\n\nin vec2 TexCoords;\nout vec4 FragColor;\n\nuniform vec2 ViewportSize;\n\nuniform float Time;\n\nvoid main()\n{\n\tFragColor = vec4(vec3(0), 1);\n}");

        ImGui::Begin("Fragment Shader");

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        const auto textSize = ImVec2(-FLT_MIN, -ImGui::GetTextLineHeight() * 5);
        InputTextCallback_UserData ud;
        ud.Str = &m_Renderer->m_Shader->m_FragmentShaderSource;
        ImGui::InputTextMultiline("User Input", ud.Str, textSize, flags, Callbacks::InputTextCallback, &ud);

        if (ImGui::Button("Recompile")) {
            std::cout << "Recompiling..." << std::endl;
            m_Renderer->m_Shader->CompileShaderProgram();
        }
        ImGui::End();
    }

    {
        ImGui::Begin("Render Window");
        const auto viewportSize = ImGui::GetContentRegionAvail();
        m_Renderer->m_ViewportSize.x = static_cast<int>(viewportSize.x);
        m_Renderer->m_ViewportSize.y = static_cast<int>(viewportSize.y);
        m_Window->m_ViewWidth = static_cast<int>(viewportSize.x);
        m_Window->m_ViewHeight = static_cast<int>(viewportSize.y);

        m_Renderer->Render();
        m_Renderer->Draw();

        ImGui::Image(reinterpret_cast<ImTextureID>(m_Renderer->m_TextureColorBuffer),
            ImVec2(
                static_cast<float>(m_Renderer->m_FramebufferSize.x),
                static_cast<float>(m_Renderer->m_FramebufferSize.y)),
            ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        ImGui::End();
    }
    ImGui::End();

    // Rendering
    ImGui::Render();
}

void GUI::LoopTeardown() const {
    // Post-render
    int display_w, display_h;
    m_Window->GetWindowSize(&display_w, &display_h);
    m_Renderer->PostRender(display_w, display_h);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = Window::GetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        Window::BackupCurrentContext(backup_current_context);
    }
}

void GUI::ShowMenuBar() const {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save", "Ctrl+S");
            ImGui::MenuItem("Exit", "Alt+F4", &m_Window->m_ShouldClose);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Renderer")) {
            if (ImGui::BeginMenu("OpenGL")) {
                ImGui::Text("Version");
                ImGui::Separator();
                // ImGui::SeparatorText("Version");
                ImGui::MenuItem("330");
                ImGui::MenuItem("450");
                ImGui::EndMenu();
            }
            ImGui::MenuItem("Vulkan");
            ImGui::MenuItem("DirectX");
            ImGui::MenuItem("Metal");
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}
