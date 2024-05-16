//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <functional>

class GLFWwindow;

class Window {
public:
    Window(int viewWidth, int viewHeight);
    ~Window();

    void BeginLoop(const std::function<void(void)> &fcn) const;
    void GetWindowSize(int* x, int* y) const;
    static double GetTime();
    static void BackupCurrentContext();

    int m_ViewWidth;
    int m_ViewHeight;
    GLFWwindow* m_Window = nullptr;
};

#endif //WINDOW_H
