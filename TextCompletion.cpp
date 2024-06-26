//
// Created by Scott Kirila on 5/28/2024.
//

#include "TextCompletion.h"

#include <algorithm>
#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"

TextCompletion::TextCompletion()
{
    std::ranges::sort(m_Dictionary);
}

std::string TextCompletion::GetCurrentWord(const ImGuiInputTextCallbackData *CallbackData)
{
    char *wordEnd = CallbackData->Buf + CallbackData->CursorPos;
    char *wordStart = wordEnd;
    while (wordStart > CallbackData->Buf)
    {
        if (!isalnum(wordStart[-1]) && wordStart[-1] != '_') break;
        wordStart--;
    }

    const int StringLength = static_cast<int>(wordEnd - wordStart);
    auto currentWord = std::string(wordStart, StringLength);

    m_CurrentWordStart = wordStart;
    m_CurrentWordEnd = wordEnd;

    return currentWord;
}

void TextCompletion::PopulateMatches(const ImGuiInputTextCallbackData *CallbackData)
{
    if (m_DoComplete) return;

    ClearResults();
    GetCurrentWord(CallbackData);

    if (const int StringLength = static_cast<int>(m_CurrentWordEnd - m_CurrentWordStart); StringLength != 0)
    {
        for (const auto &word: m_Dictionary)
        {
            if (word.compare(0, StringLength, m_CurrentWordStart, StringLength) == 0 &&
                (word.size() > StringLength))
            {
                m_Matches.push_back(word);
            }
        }
    }

    if (!m_Matches.empty())
    {
        m_SelectedMatch = m_Matches[0];
        m_CanComplete = true;
    }
}

// Should be called every frame.
void TextCompletion::DisplayMatches(const ImGuiInputTextCallbackData *CallbackData)
{
    if (m_Matches.empty()) return;

    // Clear results if cursor moves away from current word
    m_WordStart = static_cast<int>(m_CurrentWordStart - CallbackData->Buf);

    if (const int wordEnd = static_cast<int>(m_CurrentWordEnd - CallbackData->Buf);
        CallbackData->CursorPos < m_WordStart || CallbackData->CursorPos > wordEnd)
    {
        ClearResults();
        return;
    }

    // Display list
    ImGui::SetNextWindowPos(ImVec2(
            CallbackData->Ctx->PlatformImeData.InputPos.x,
            CallbackData->Ctx->PlatformImeData.InputPos.y + ImGui::GetTextLineHeight()
    ));

    if (ImGui::BeginTooltip())
    {
        const size_t start = m_CurrentIndex <= m_MaxDisplayMatches ? 0 : m_CurrentIndex - m_MaxDisplayMatches;
        const size_t end = m_CurrentIndex < m_MaxDisplayMatches ? 1 + 2 * m_MaxDisplayMatches : m_CurrentIndex +
                                                                                             m_MaxDisplayMatches + 1;

        for (size_t i = start; i < m_Matches.size() && i < end; i++)
        {
            const bool is_selected = (i == m_CurrentIndex);

            if (ImGui::Selectable(m_Matches[i].c_str(), is_selected))
            {
                m_CurrentIndex = i;
                m_SelectedMatch = m_Matches[i];
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndTooltip();
    }
}

void TextCompletion::DoComplete(ImGuiInputTextCallbackData *data)
{
    if (!m_DoComplete) return;

    const auto length = static_cast<int>(m_CurrentWordEnd - m_CurrentWordStart);
    data->DeleteChars(m_WordStart, length);
    data->InsertChars(data->CursorPos, m_Matches[m_CurrentIndex].c_str());

    ClearResults();
}

void TextCompletion::ClearResults()
{
    m_CanComplete = false;
    m_DoComplete = false;
    m_SelectedMatch.clear();
    m_Matches.clear();
    m_CurrentWordStart = nullptr;
    m_CurrentWordEnd = nullptr;
    m_CurrentIndex = 0;
}
