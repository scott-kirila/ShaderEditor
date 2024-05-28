
#include <iostream>

#include "GUI.h"
#include "RenderingOpenGL.h"
#include "Window.h"

constexpr int g_ViewWidth = 1920;
constexpr int g_ViewHeight = 1080;
auto g_GlslVersion = "#version 330\n";

int main() {

    const auto window = std::make_shared<Window>(g_ViewWidth, g_ViewHeight);
    auto renderer = Rendering::OpenGL(window.get(), g_GlslVersion);
    auto gui = GUI(window.get(), &renderer, g_ViewWidth, g_ViewHeight, g_GlslVersion);

    gui.Loop();

    std::cout << "End\n";

    return EXIT_SUCCESS;
}
