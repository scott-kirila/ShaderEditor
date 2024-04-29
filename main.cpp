#include "GUI.h"
#include "Renderer.h"
#include "Shader.h"
#include "Window.h"

constexpr int g_ViewWidth = 1920;
constexpr int g_ViewHeight = 1080;
const char* g_GlslVersion = "#version 330";

void TestFunction() {
    //std::cout << "Test\n";
}

int main() {

    Window window = Window(g_ViewWidth, g_ViewHeight);

    void (*ptr)() = &TestFunction;

    GUI gui = GUI(window, g_ViewWidth, g_ViewHeight, g_GlslVersion);

    Shader shader = Shader(g_GlslVersion);
    Renderer renderer = Renderer();

    window.BeginLoop(ptr);

    return EXIT_SUCCESS;
}
