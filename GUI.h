//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef GUI_H
#define GUI_H

#include "Callbacks.h"

#include "imgui.h"
#include <string>

class Window;

namespace ImGui {
    inline bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;

        return InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags | ImGuiInputTextFlags_CallbackResize, Callbacks::TextResizeCallback, (void*)str);
    }
}

class GUI {
public:
    GUI(Window& window, float viewWidth, float viewHeight, const char* glslVersion);
    ~GUI();

private:
    ImVec2 m_FramebufferSize;
    ImVec4 m_ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool m_ShowDemoWindow = false;
};



#endif //GUI_H
