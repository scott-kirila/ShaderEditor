//
// Created by hetan on 2024/04/28.
//

#include "Callbacks.h"

#include <cstdio>
#include <imgui.h>
#include <string>

int Callbacks::TextResizeCallback(ImGuiInputTextCallbackData *data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        const auto UserData = static_cast<std::string*>(data->UserData);

        IM_ASSERT(UserData->c_str() == data->Buf);

        UserData->resize(data->BufTextLen);
        data->Buf = const_cast<char *>(UserData->c_str());
    }

    return 0;
}

void Callbacks::GlfwErrorCallback(const int error, const char *description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
