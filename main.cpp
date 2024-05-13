#include "GUI.h"
#include "Renderer.h"
#include "Shader.h"
#include "Window.h"

constexpr int g_ViewWidth = 1920;
constexpr int g_ViewHeight = 1080;
const char* g_GlslVersion = "#version 330";

Window window = Window(g_ViewWidth, g_ViewHeight);
Renderer renderer = Renderer(&window);
Shader shader = Shader(g_GlslVersion);
GUI gui = GUI(&window, &renderer, &shader, g_ViewWidth, g_ViewHeight, g_GlslVersion);

void TestFunction() {
    //std::cout << "Test\n";
    gui.LoopCall();
}

int main() {

    void (*ptr)() = &TestFunction;
    // void (*ptr)() = &(gui.LoopCall);

    window.BeginLoop(ptr);

    return EXIT_SUCCESS;
}
