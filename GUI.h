//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef GUI_H
#define GUI_H

#include "imgui.h"

#include <string>

#include "Callbacks.h"
#include "RenderingOpenGL.h"

class Shader;
class Window;
namespace Rendering {
    class OpenGL;
}

namespace ImGui {
    inline bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        return InputTextMultiline(label, const_cast<char *>(str->c_str()), str->capacity() + 1, size, flags | ImGuiInputTextFlags_CallbackResize, Callbacks::TextResizeCallback, (void*)str);
    }
}

class GUI {
public:
    GUI(Window* window, Rendering::OpenGL* renderer, float viewWidth, float viewHeight, const char* glslVersion);
    ~GUI();

    void Loop();

private:
    void LoopSetup();
    void LoopBody() const;
    void LoopTeardown() const;

    const char* m_GlslVersion;
    ImVec2 m_FramebufferSize;
    Window* m_Window;
    Rendering::OpenGL* m_Renderer;
    ImGuiIO* m_IO;
    bool m_ShowDemoWindow = false;
};

#endif //GUI_H
