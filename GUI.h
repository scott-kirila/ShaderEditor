//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef GUI_H
#define GUI_H

#include "imgui.h"

class Shader;
class Window;
namespace Rendering { class OpenGL; }

class GUI {
public:
    GUI(Window* window, Rendering::OpenGL* renderer, float viewWidth, float viewHeight, const char* glslVersion);
    ~GUI();

    void Loop();

private:
    void LoopSetup();
    void LoopBody() const;
    void LoopTeardown() const;

    void ShowMenuBar() const;

    const char* m_GlslVersion;
    ImVec2 m_FramebufferSize;
    Window* m_Window;
    Rendering::OpenGL* m_Renderer;
    ImGuiIO* m_IO;
    bool m_ShowDemoWindow = true;
};

#endif //GUI_H
