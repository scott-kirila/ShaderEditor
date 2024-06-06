//
// Created by hetan on 5/28/2024.
//
#ifdef _WIN64
#include "Windows.h"
#endif

#include "imgui.h"

#include "TextCompletion.h"

#include <imgui_internal.h>
#include <iostream>

#include "Callbacks.h"

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
    if (m_DoComplete) return;

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
        m_CanComplete = true;
    }
}

// Should be called every frame.
void TextCompletion::DisplayMatches(ImGuiInputTextCallbackData *CallbackData) {
    if (m_Matches.empty()) return;

    const float linePadding = ImGui::GetStyle().ItemSpacing.y;
    CalcListPos(CallbackData);

    // Clear results if cursor moves away from current word
    m_WordStart = static_cast<int>(m_CurrentWordStart - CallbackData->Buf);
    const int wordEnd = static_cast<int>(m_CurrentWordEnd - CallbackData->Buf);

    if (CallbackData->CursorPos < m_WordStart || CallbackData->CursorPos > wordEnd) {
        ClearResults();
        return;
    }

    // Display list
    ImGui::SetNextWindowPos(ImVec2(
        CallbackData->Ctx->PlatformImeData.InputPos.x,
        CallbackData->Ctx->PlatformImeData.InputPos.y + ImGui::GetTextLineHeight()
        ));

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

void TextCompletion::DoComplete(ImGuiInputTextCallbackData *data) {
    if (!m_DoComplete) return;

    const auto length = static_cast<int>(m_CurrentWordEnd - m_CurrentWordStart);
    data->DeleteChars(m_WordStart, length);
    data->InsertChars(data->CursorPos, m_Matches[m_CurrentIndex].c_str());
    data->InsertChars(data->CursorPos, " ");

    ClearResults();
}

void TextCompletion::ClearResults() {
    m_CanComplete = false;
    m_DoComplete = false;
    m_SelectedMatch.clear();
    m_Matches.clear();
    m_CurrentWordStart = nullptr;
    m_CurrentWordEnd = nullptr;
    m_CurrentIndex = 0;
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
