//
// Created by hetan on 2024/04/28.
//

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <iostream>

#include "imgui.h"

struct InputTextCallback_UserData {

    std::string* Str{};
    ImGuiInputTextCallback ChainCallback{};
    void* ChainCallbackUserData{};
};

class Callbacks {
public:
    static int InputTextCallback(ImGuiInputTextCallbackData* data);

    static int TextResizeCallback(ImGuiInputTextCallbackData* data);

    static void GlfwErrorCallback(int error, const char* description);
};

#endif //CALLBACKS_H
