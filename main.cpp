
#include <iostream>

#include "GUI.h"
#include "RenderingOpenGL.h"
#include "Window.h"

constexpr int VIEW_WIDTH = 1920;
constexpr int VIEW_HEIGHT = 1080;
#if _WIN64
auto g_GlslVersion = "#version 460\n";
#elif __APPLE__
auto g_GlslVersion = "#version 410\n";
#endif

int main()
{
    const auto window = std::make_shared<Window>(VIEW_WIDTH, VIEW_HEIGHT);
    const auto renderer = std::make_shared<Rendering::OpenGL>(window.get(), g_GlslVersion);
    auto gui = GUI(window, renderer, VIEW_WIDTH, VIEW_HEIGHT, g_GlslVersion);

    gui.Loop();

    std::cout << "End\n";

    return EXIT_SUCCESS;
}
