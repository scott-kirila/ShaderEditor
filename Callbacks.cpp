//
// Created by Scott Kirila on 2024/04/28.
//

#include "Callbacks.h"

#include <cstdio>
#include <string>
#include <vector>

#include "imgui_internal.h"

#include "TextCompletion.h"

int Callbacks::InputTextCallback(ImGuiInputTextCallbackData *data)
{
    static TextCompletion text_completion{};
    const auto *user_data = static_cast<InputTextCallback_UserData *>(data->UserData);

    switch (data->EventFlag)
    {
        case ImGuiInputTextFlags_CallbackAlways:
            text_completion.DisplayMatches(data);
            text_completion.DoComplete(data);
            break;
        case ImGuiInputTextFlags_CallbackEdit:
            text_completion.PopulateMatches(data);
            break;
        case ImGuiInputTextFlags_CallbackCharFilter:
            if (data->EventChar == '\t' && text_completion.m_CanComplete)
            {
                text_completion.m_CurrentIndex++;
                text_completion.m_CurrentIndex %= static_cast<int>(text_completion.m_Matches.size());

                return 1;
            }

            if (data->EventChar == '\n' && text_completion.m_CanComplete)
            {
                text_completion.m_DoComplete = true;
                return 1;
            }

            break;
        case ImGuiInputTextFlags_CallbackResize:
        {
            // Resize string callback
            // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
            std::string *str = user_data->Str;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);

            // ReSharper disable once CppDFALocalValueEscapesFunction
            data->Buf = const_cast<char *>(str->c_str());
            break;
        }
        default:
            if (user_data->ChainCallback)
            {
                // Forward to user callback, if any
                data->UserData = user_data->ChainCallbackUserData;
                return user_data->ChainCallback(data);
            }
            break;
    }

    return 0;
}

int Callbacks::TextResizeCallback(ImGuiInputTextCallbackData *data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        const auto str = static_cast<std::string *>(data->UserData);
        IM_ASSERT(data->Buf == str->c_str());

        str->resize(data->BufTextLen);
        data->Buf = new char[data->BufTextLen];
#if _WIN64
        strcpy_s(data->Buf, data->BufTextLen, str->c_str());
#elif __APPLE__
        std::strncpy(data->Buf, str->c_str(), data->BufTextLen);
#endif
    }

    return 0;
}

void Callbacks::GlfwErrorCallback(const int error, const char *description)
{
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
