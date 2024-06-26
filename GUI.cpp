//
// Created by Scott Kirila on 2024/04/29.
//

#include "GUI.h"

#include <iostream>

#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Callbacks.h"
#include "Shader.h"

namespace ImGui
{
    bool InputTextMultiline(const char *label, std::string *str, const ImVec2 &size, ImGuiInputTextFlags flags,
                            const ImGuiInputTextCallback callback, void *userData)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;
        flags |= ImGuiInputTextFlags_CallbackAlways;
        flags |= ImGuiInputTextFlags_CallbackCharFilter;
        flags |= ImGuiInputTextFlags_CallbackEdit;

        InputTextCallback_UserData callbackUserData;
        callbackUserData.Str = str;
        callbackUserData.ChainCallback = callback;
        callbackUserData.ChainCallbackUserData = userData;

        return InputTextMultiline(label, const_cast<char *>(str->c_str()), str->capacity() + 1,
                                  size, flags, callback, &callbackUserData);
    }
}

GUI::GUI(const std::shared_ptr<Window> &window, const std::shared_ptr<Rendering::OpenGL> &renderer,
         const float viewWidth, const float viewHeight, const char *glslVersion)
        : m_GlslVersion(glslVersion), m_FramebufferSize(viewWidth, viewHeight),
          m_Window(window), m_Renderer(renderer)
{
    // Set up Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.Fonts->AddFontFromFileTTF("../OpenSans-Regular.ttf", 32.0f);

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        style.Colors[ImGuiCol_FrameBg] = ImVec4{0.1, 0.1, 0.1, 1.0};
        style.Colors[ImGuiCol_Button] = ImVec4{0.2, 0.2, 0.2, 1.0};
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4{0.15, 0.0, 0.25, 1.0};
        style.Colors[ImGuiCol_ButtonActive] = ImVec4{0.10, 0.0, 0.2, 1.0};
        style.Colors[ImGuiCol_TabActive] = ImVec4{0.15, 0.15, 0.15, 1.0};
        style.Colors[ImGuiCol_TabHovered] = ImVec4{0.2, 0.2, 0.2, 1.0};
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.1, 0.1, 0.1, 1.0};
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4{0.0, 0.0, 0.0, 1.0};

        style.FrameRounding = 8.0f;

        style.WindowPadding = ImVec2(5.0f, 1.0f);
        style.FramePadding = ImVec2(5.0f, 2.0f);
        style.ItemSpacing = ImVec2(10.0f, 1.0f);
        style.ItemInnerSpacing = ImVec2(2.0f, 2.0f);
        style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
        style.IndentSpacing = 21.0f;
        style.ScrollbarSize = 14.0f;
        style.GrabMinSize = 14.0f;

        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;
        style.TabBorderSize = 1.0f;
        style.TabBarBorderSize = 1.0f;

        style.WindowRounding = 6.0f;
        style.ChildRounding = 6.0f;
        style.FrameRounding = 8.0f;
        style.PopupRounding = 0.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabRounding = 0.0f;
        style.TabRounding = 12.0f;
    }

    m_IO = &io;

    // Set up backends
    ImGui_ImplGlfw_InitForOpenGL(window->m_Window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

GUI::~GUI()
{
    std::cout << "GUI\n";
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::Loop()
{
    m_Window->BeginLoop([this]()
                        {
                            LoopSetup();
                            LoopBody();
                            LoopTeardown();
                        });
}

void GUI::LoopSetup()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (m_ShowDemoWindow)
    {
        ImGui::ShowDemoWindow(&m_ShowDemoWindow);
    }

    // DockSpace
    static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Shader Editor", nullptr, windowFlags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    const ImGuiID dockSpaceId = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
}

void GUI::LoopBody() const
{
    {
        ShowMenuBar();

        static std::string fragmentShaderString = m_GlslVersion + std::string(
                "\n\n"
                "in vec2 TexCoords;\n"
                "out vec4 FragColor;\n\n"
                "uniform vec2 ViewportSize;\n\n"
                "uniform float Time;\n\n"
                "void main()\n"
                "{\n"
                "\tFragColor = vec4(vec3(0), 1);\n"
                "}");

        ImGui::Begin("Fragment Shader");

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        const auto textSize = ImVec2(-FLT_MIN, -ImGui::GetTextLineHeight() * 5);
        InputTextCallback_UserData ud;
        ud.Str = &m_Renderer->m_Shader->m_FragmentShaderSource;
        ImGui::InputTextMultiline("User Input", ud.Str, textSize, flags, Callbacks::InputTextCallback, &ud);

        if (ImGui::Button("Recompile"))
        {
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

        ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(m_Renderer->m_TextureColorBuffer)),
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

void GUI::LoopTeardown() const
{
    // Post-render
    int displayWidth, displayHeight;
    m_Window->GetWindowSize(&displayWidth, &displayHeight);
    m_Renderer->PostRender(displayWidth, displayHeight);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backupCurrentContext = Window::GetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        Window::BackupCurrentContext(backupCurrentContext);
    }
}

void GUI::ShowMenuBar() const
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save", "Ctrl+S");
            ImGui::MenuItem("Exit", "Alt+F4", &m_Window->m_ShouldClose);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Renderer"))
        {
            if (ImGui::BeginMenu("OpenGL"))
            {
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
