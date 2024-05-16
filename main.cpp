#include "GUI.h"
#include "Renderer.h"
#include "Shader.h"
#include "Window.h"

#include <iostream>

constexpr int g_ViewWidth = 1920;
constexpr int g_ViewHeight = 1080;
const char* g_GlslVersion = "#version 330\n";

int main() {
    Window window = Window(g_ViewWidth, g_ViewHeight);
    Shader shader = Shader(g_GlslVersion);
    Renderer renderer = Renderer(&window, &shader);
    GUI gui = GUI(&window, &renderer, g_ViewWidth, g_ViewHeight, g_GlslVersion);

    gui.Loop();

    std::cout << "End\n";

    return EXIT_SUCCESS;
}
