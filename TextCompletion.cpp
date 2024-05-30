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

void TextCompletion::PopulateMatches(const ImGuiInputTextCallbackData* CallbackData) {
    ClearResults();
    GetCurrentWord(CallbackData);

    if (const int StringLength = static_cast<int>(CurrentWordEnd - CurrentWordStart); StringLength != 0) {
        for (const auto& word : Dictionary) {
            if (word.compare(0, StringLength, CurrentWordStart, StringLength) == 0) {
                Matches.push_back(word);
            }
        }
    }
}

// Should be called every frame.
void TextCompletion::DisplayMatches(const ImGuiInputTextCallbackData *CallbackData) {
    if (Matches.empty()) return;

    const float linePadding = ImGui::GetTextLineHeightWithSpacing() - ImGui::GetTextLineHeight();
    CalcListPos(CallbackData);

    // Clear results if cursor moves away from current word
    const int wordStart = static_cast<int>(CurrentWordStart - CallbackData->Buf);
    const int wordEnd = static_cast<int>(CurrentWordEnd - CallbackData->Buf);

    if (CallbackData->CursorPos < wordStart || CallbackData->CursorPos > wordEnd) {
        ClearResults();
        return;
    }

    // Display
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetItemRectMin().x + xPos,
        ImGui::GetItemRectMin().y + yPos + linePadding - ImGui::GetScrollY()));
    ImGui::BeginTooltip();
    for (const auto& match : Matches) {
        ImGui::Selectable(match.c_str(), false);
    }
    ImGui::EndTooltip();
}

void TextCompletion::ClearResults() {
    Matches.clear();
    CurrentWordStart = nullptr;
    CurrentWordEnd = nullptr;
}

void TextCompletion::CalcListPos(const ImGuiInputTextCallbackData* CallbackData) {
    int linePos{};
    for (int i = CallbackData->CursorPos - 1; CallbackData->Buf[i] != '\n' && i > 0; i--) {
        linePos++;
    }

    xPos = ImGui::CalcTextSize(CallbackData->Buf + CallbackData->CursorPos - linePos, CallbackData->Buf + CallbackData->CursorPos).x;

    yPos = ImGui::CalcTextSize(CallbackData->Buf, CallbackData->Buf + CallbackData->CursorPos).y;
    yPos = (linePos != 0) ? yPos : yPos + ImGui::GetFontSize();
}
