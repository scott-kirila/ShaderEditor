//
// Created by hetan on 5/28/2024.
//

#include "imgui.h"

#include "TextCompletion.h"

std::string TextCompletion::GetCurrentWord(const ImGuiInputTextCallbackData* CallbackData) {
    char* wordEnd = CallbackData->Buf + CallbackData->CursorPos;
    char* wordStart = wordEnd;
    while (wordStart > CallbackData->Buf) {
        if (!isalnum(wordStart[-1])) break;
        wordStart--;
    }

    const int StringLength = static_cast<int>(wordEnd - wordStart);
    auto currentWord = std::string(wordStart, StringLength);

    CurrentWordStart = wordStart;
    CurrentWordEnd = wordEnd;

    return currentWord;
}
