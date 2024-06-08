//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_NONE

#include <functional>

#include "GLFW/glfw3.h"

class Window {
public:
    Window(int viewWidth, int viewHeight);
    ~Window();

    void BeginLoop(const std::function<void(void)> &fcn) const;
    void GetWindowSize(int* x, int* y) const;

    static double GetTime();

    static GLFWwindow* GetCurrentContext();
    static void BackupCurrentContext(GLFWwindow* current_context);
    static void* GetProcAddress(const char* procname);

    int m_ViewWidth{};
    int m_ViewHeight{};
    GLFWwindow* m_Window;
    bool m_ShouldClose{};

};

#endif //WINDOW_H
