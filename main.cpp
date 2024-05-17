
#include <iostream>

#include "GUI.h"
#include "RenderingOpenGL.h"
#include "Shader.h"
#include "Window.h"

constexpr int g_ViewWidth = 1920;
constexpr int g_ViewHeight = 1080;
auto g_GlslVersion = "#version 330\n";

int main() {

    auto window = Window(g_ViewWidth, g_ViewHeight);
    auto shader = Shader(g_GlslVersion);
    auto renderer = Rendering::OpenGL(&window, &shader);
    auto gui = GUI(&window, &renderer, g_ViewWidth, g_ViewHeight, g_GlslVersion);

    gui.Loop();

    std::cout << "End\n";

    return EXIT_SUCCESS;
}
