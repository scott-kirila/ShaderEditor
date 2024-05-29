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

void TextCompletion::PopulateMatches() {
    Matches.clear();

    if (const int StringLength = static_cast<int>(CurrentWordEnd - CurrentWordStart); StringLength != 0) {
        for (const auto& word : Dictionary) {
            if (word.compare(0, StringLength, CurrentWordStart, StringLength) == 0) {
                Matches.push_back(word);
            }
        }
    }
}
