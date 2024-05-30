//
// Created by hetan on 5/28/2024.
//

#include "imgui.h"

#include "TextCompletion.h"

#include <imgui_internal.h>
#include <iostream>

std::string TextCompletion::GetCurrentWord(const ImGuiInputTextCallbackData* CallbackData) {
    char* wordEnd = CallbackData->Buf + CallbackData->CursorPos;
    char* wordStart = wordEnd;
    while (wordStart > CallbackData->Buf) {
        if (!isalnum(wordStart[-1])) break;
        wordStart--;
    }

    const int StringLength = static_cast<int>(wordEnd - wordStart);
    auto currentWord = std::string(wordStart, StringLength);

    m_CurrentWordStart = wordStart;
    m_CurrentWordEnd = wordEnd;

    return currentWord;
}

void TextCompletion::PopulateMatches(const ImGuiInputTextCallbackData* CallbackData) {
    ClearResults();
    GetCurrentWord(CallbackData);

    if (const int StringLength = static_cast<int>(m_CurrentWordEnd - m_CurrentWordStart); StringLength != 0) {
        for (const auto& word : m_Dictionary) {
            if (word.compare(0, StringLength, m_CurrentWordStart, StringLength) == 0) {
                m_Matches.push_back(word);
            }
        }
    }

    if (!m_Matches.empty()) {
        m_SelectedMatch = m_Matches[0];
        canComplete = true;
    }
}

// Should be called every frame.
void TextCompletion::DisplayMatches(const ImGuiInputTextCallbackData *CallbackData) {
    if (m_Matches.empty()) return;

    const float linePadding = ImGui::GetStyle().ItemSpacing.y;
    CalcListPos(CallbackData);

    // Clear results if cursor moves away from current word
    const int wordStart = static_cast<int>(m_CurrentWordStart - CallbackData->Buf);
    const int wordEnd = static_cast<int>(m_CurrentWordEnd - CallbackData->Buf);

    // if (CallbackData->CursorPos < wordStart || CallbackData->CursorPos > wordEnd) {
    //     ClearResults();
    //     return;
    // }

    // Display list
    ImGui::SetNextWindowPos(ImVec2(
        ImGui::GetItemRectMin().x + m_XPos,
        ImGui::GetItemRectMin().y + m_YPos + linePadding - ImGui::GetScrollY()));

    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false)) {
        m_CurrentIndex++;
        ImGui::GetIO().AddKeyEvent(ImGuiKey_UpArrow, false);
    }

    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false)) {
        m_CurrentIndex--;
        ImGui::GetIO().AddKeyEvent(ImGuiKey_DownArrow, false);
    }

    m_CurrentIndex += m_Matches.size();
    m_CurrentIndex %= m_Matches.size();

    if(ImGui::BeginTooltip()) {
        for (int i = 0; i < m_Matches.size(); i++) {
            const bool is_selected = (i == m_CurrentIndex);
            if(ImGui::Selectable(m_Matches[i].c_str(), is_selected)) {
                m_CurrentIndex = i;
                m_SelectedMatch = m_Matches[i];
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndTooltip();
    }
}

void TextCompletion::ClearResults() {
    canComplete = false;
    m_SelectedMatch.clear();
    m_Matches.clear();
    m_CurrentWordStart = nullptr;
    m_CurrentWordEnd = nullptr;
}

void TextCompletion::CalcListPos(const ImGuiInputTextCallbackData* CallbackData) {
    int linePos{};
    for (int i = CallbackData->CursorPos - 1; CallbackData->Buf[i] != '\n' && i > 0; i--) {
        linePos++;
    }

    m_XPos = ImGui::CalcTextSize(CallbackData->Buf + CallbackData->CursorPos - linePos,
        CallbackData->Buf + CallbackData->CursorPos).x;

    m_YPos = ImGui::CalcTextSize(CallbackData->Buf, CallbackData->Buf + CallbackData->CursorPos).y;
    m_YPos = (linePos != 0) ? m_YPos : m_YPos + ImGui::GetFontSize();
}
