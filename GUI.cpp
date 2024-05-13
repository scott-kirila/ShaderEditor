//
// Created by Scott Kirila on 2024/04/29.
//

// SHOULD ONLY DEPEND ON DEAR-IMGUI
// BUT ALSO DEPENDS ON OPENGL. NEED TO CREATE WRAPPERS IN Renderer TO AVOID THIS

#include "GUI.h"
#include "Shader.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Renderer.h"
#include "Shader.h"
#include "Window.h"

#include <iostream>

GUI::GUI(Window* window, Renderer* renderer, Shader* shader, float viewWidth, float viewHeight, const char* glslVersion) :
m_GlslVersion(glslVersion), m_FramebufferSize(viewWidth, viewHeight),
m_Window(window), m_Renderer(renderer), m_Shader(shader) {
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

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = false;
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::LoopCall() {
    SetUpWindow();
}

void GUI::SetUpWindow() {
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

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    {
        static std::string fragmentShaderString = m_GlslVersion + std::string("\n\nin vec2 TexCoords;\nout vec4 FragColor;\n\nuniform vec2 ViewportSize;\n\nuniform float Time;\n\nvoid main()\n{\n\tFragColor = vec4(vec3(0), 1);\n}");

        ImGui::Begin("Fragment Shader");

        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        ImGui::InputTextMultiline("User Input", &m_Shader->m_FragmentShaderSource, ImVec2(-FLT_MIN, -ImGui::GetTextLineHeight() * 5), flags);

        if (ImGui::Button("Recompile")) {
            std::cout << "Recompiling..." << std::endl;

            m_Shader->m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
            const char* source = m_Shader->m_VertexShaderSource.c_str();
            glShaderSource(m_Shader->m_VertexShader, 1, &source, nullptr);
            glCompileShader(m_Shader->m_VertexShader);

            int success;
            char infoLog[512];

            glGetShaderiv(m_Shader->m_VertexShader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(m_Shader->m_VertexShader, 512, nullptr, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            try {
                RefreshShader();
            } catch (const std::exception& e) {
                ReloadShader();
            }
        }
        ImGui::End();
    }

    {
        ImGui::Begin("Render Window");
        auto viewportSize = ImGui::GetContentRegionAvail();

        if (m_FramebufferSize.x != viewportSize.x || m_FramebufferSize.y != viewportSize.y) {
            m_FramebufferSize = viewportSize;

            glDeleteFramebuffers(1, &m_Renderer->m_FrameBuffer);
            glDeleteRenderbuffers(1, &m_Renderer->m_RenderBuffer);

            glGenFramebuffers(1, &m_Renderer->m_FrameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, m_Renderer->m_FrameBuffer);

            glViewport(0, 0, viewportSize.x, viewportSize.y);

            glBindTexture(GL_TEXTURE_2D, m_Renderer->m_TextureColorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Renderer->m_FrameBufferSize.x, m_Renderer->m_FrameBufferSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Renderer->m_TextureColorBuffer, 0);

            glGenRenderbuffers(1, &m_Renderer->m_RenderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, m_Renderer->m_RenderBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Renderer->m_FrameBufferSize.x, m_Renderer->m_FrameBufferSize.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Renderer->m_RenderBuffer);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, m_Renderer->m_FrameBuffer);
            glViewport(0, 0, viewportSize.x, viewportSize.y);
        }

        // Draw
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glUseProgram(m_Shader->m_ShaderProgram);
        int u_ViewportSize = glGetUniformLocation(m_Shader->m_ShaderProgram, "ViewportSize");
        glUniform2f(u_ViewportSize, viewportSize.x, viewportSize.y);
        int u_Time = glGetUniformLocation(m_Shader->m_ShaderProgram, "Time");
        glUniform1f(u_Time, glfwGetTime());

        glBindVertexArray(m_Renderer->m_ArrayBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ImGui::Image((void*)m_Renderer->m_TextureColorBuffer, ImVec2(m_Renderer->m_FrameBufferSize.x, m_Renderer->m_FrameBufferSize.y), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
        ImGui::End();
    }
    ImGui::End();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window->m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(m_ClearColor.x * m_ClearColor.w, m_ClearColor.y * m_ClearColor.w, m_ClearColor.z * m_ClearColor.w, m_ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

}

void GUI::ReloadShader() {
    int success;
    char infoLog[512];
    m_Shader->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* newFragmentShader = m_Shader->m_FragmentShaderSource.c_str();
    glShaderSource(m_Shader->m_FragmentShader, 1, &newFragmentShader, nullptr);
    glCompileShader(m_Shader->m_FragmentShader);
    glGetShaderiv(m_Shader->m_FragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(m_Shader->m_FragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_Shader->m_ShaderProgram = glCreateProgram();
    glAttachShader(m_Shader->m_ShaderProgram, m_Shader->m_VertexShader);
    glAttachShader(m_Shader->m_ShaderProgram, m_Shader->m_FragmentShader);
    glLinkProgram(m_Shader->m_ShaderProgram);
    glGetProgramiv(m_Shader->m_ShaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(m_Shader->m_ShaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(m_Shader->m_VertexShader);
    glDeleteShader(m_Shader->m_FragmentShader);
}

void GUI::RefreshShader() {
    int success;
    char infoLog[512];
    m_Shader->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* newFragmentShader = m_Shader->m_FragmentShaderSource.c_str();
    glShaderSource(m_Shader->m_FragmentShader, 1, &newFragmentShader, nullptr);
    glCompileShader(m_Shader->m_FragmentShader);
    glGetShaderiv(m_Shader->m_FragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(m_Shader->m_FragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    m_Shader->m_ShaderProgram = glCreateProgram();
    glAttachShader(m_Shader->m_ShaderProgram, m_Shader->m_VertexShader);
    glAttachShader(m_Shader->m_ShaderProgram, m_Shader->m_FragmentShader);
    glLinkProgram(m_Shader->m_ShaderProgram);
    glGetProgramiv(m_Shader->m_ShaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(m_Shader->m_ShaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(m_Shader->m_VertexShader);
    glDeleteShader(m_Shader->m_FragmentShader);
}

