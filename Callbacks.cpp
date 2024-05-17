//
// Created by hetan on 2024/04/28.
//

#include <cstdio>
#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui_internal.h"
#include "imstb_textedit.h"

#include "Callbacks.h"

#define STB_TEXTEDIT_K_LEFT         0x200000 // keyboard input to move cursor left
#define STB_TEXTEDIT_K_RIGHT        0x200001 // keyboard input to move cursor right
#define STB_TEXTEDIT_K_UP           0x200002 // keyboard input to move cursor up
#define STB_TEXTEDIT_K_DOWN         0x200003 // keyboard input to move cursor down
#define STB_TEXTEDIT_K_LINESTART    0x200004 // keyboard input to move cursor to start of line
#define STB_TEXTEDIT_K_LINEEND      0x200005 // keyboard input to move cursor to end of line
#define STB_TEXTEDIT_K_TEXTSTART    0x200006 // keyboard input to move cursor to start of text
#define STB_TEXTEDIT_K_TEXTEND      0x200007 // keyboard input to move cursor to end of text
#define STB_TEXTEDIT_K_DELETE       0x200008 // keyboard input to delete selection or character under cursor
#define STB_TEXTEDIT_K_BACKSPACE    0x200009 // keyboard input to delete selection or character left of cursor
#define STB_TEXTEDIT_K_UNDO         0x20000A // keyboard input to perform undo
#define STB_TEXTEDIT_K_REDO         0x20000B // keyboard input to perform redo
#define STB_TEXTEDIT_K_WORDLEFT     0x20000C // keyboard input to move cursor left one word
#define STB_TEXTEDIT_K_WORDRIGHT    0x20000D // keyboard input to move cursor right one word
#define STB_TEXTEDIT_K_PGUP         0x20000E // keyboard input to move cursor up a page
#define STB_TEXTEDIT_K_PGDOWN       0x20000F // keyboard input to move cursor down a page
#define STB_TEXTEDIT_K_SHIFT        0x400000

int Callbacks::InputTextCallback(ImGuiInputTextCallbackData* data)
{
    const auto* user_data = static_cast<InputTextCallback_UserData *>(data->UserData);

    if (data->EventFlag == ImGuiInputTextFlags_CallbackAlways) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiContext& g = *GImGui;
        ImGuiIO& io = g.IO;
        const char* label = "User Input";
        const ImGuiID id = window->GetID(label);
        ImGuiInputTextState* state = ImGui::GetInputTextState(id);

        const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
        bool is_multiline = true;

        if (ImGui::IsKeyPressed(ImGuiKey_UpArrow) && is_multiline) {
            // state->OnKeyPressed(STB_TEXTEDIT_K_UP | k_mask);

        }
        // STB_TEXTEDIT_K_WORDLEFT, _LEFT
        // STB_TEXTEDIT_K_WORDRIGHT, _RIGHT
        // STB_TEXTEDIT_K_UP, _DOWN
        // STB_TEXTEDIT_K_LINESTART, _LINEEND
        // STB_TEXTEDIT_K_TEXTSTART, _TEXTEND
        // STB_TEXTEDIT_K_PGUP, _PGDOWN

        // g.PlatformImeData.InputPos ?
    }
    else if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
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
