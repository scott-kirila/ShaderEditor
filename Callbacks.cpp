//
// Created by hetan on 2024/04/28.
//

#include "Callbacks.h"

#include <cstdio>
#include <imgui.h>
#include <iostream>
#include <string>

int Callbacks::InputTextCallback(ImGuiInputTextCallbackData* data)
{
    const auto* user_data = static_cast<InputTextCallback_UserData *>(data->UserData);

    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);

        // ReSharper disable once CppDFALocalValueEscapesFunction
        data->Buf = const_cast<char *>(str->c_str());
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

int Callbacks::TextResizeCallback(ImGuiInputTextCallbackData *data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        const auto str = static_cast<std::string*>(data->UserData);
        // const auto* user_data = static_cast<InputTextCallback_UserData *>(data->UserData);

        IM_ASSERT(data->Buf == str->c_str());

        str->resize(data->BufTextLen);
        // delete[] data->Buf;
        // data->Buf = const_cast<char *>(UserData->c_str());
        data->Buf = new char[data->BufTextLen];
        strcpy_s(data->Buf, data->BufTextLen, str->c_str());
    }

    return 0;
}

void Callbacks::GlfwErrorCallback(const int error, const char *description) {
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
