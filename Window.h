//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef WINDOW_H
#define WINDOW_H

class GLFWwindow;

class Window {
public:
    Window(int viewWidth, int viewHeight);

    ~Window();

    void BeginLoop(void (*functionPtr)());



    int m_ViewWidth;
    int m_ViewHeight;
    GLFWwindow* m_Window = nullptr;
};



#endif //WINDOW_H
