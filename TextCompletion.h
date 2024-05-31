//
// Created by hetan on 5/28/2024.
//

#ifndef TEXTCOMPLETION_H
#define TEXTCOMPLETION_H
#include <list>
#include <mutex>
#include <string>
#include <vector>

class TextCompletion {
public:
    std::string GetCurrentWord(const ImGuiInputTextCallbackData* CallbackData);
    void PopulateMatches(const ImGuiInputTextCallbackData* CallbackData);
    void DisplayMatches(ImGuiInputTextCallbackData* CallbackData);

    std::vector<std::string> m_Matches{};

    bool canComplete{};
    int m_CurrentIndex{};
    char* m_CurrentWordStart{};
    char* m_CurrentWordEnd{};
    std::string m_SelectedMatch{};

private:
    void ClearResults();
    void CalcListPos(const ImGuiInputTextCallbackData* CallbackData);

    float m_XPos{};
    float m_YPos{};


    bool canInitCursorPos{true};
    bool canUpdateCursorPos{true};
    int lastCursorPos{-1};

    const std::list<std::string> m_Dictionary = {
        "vec2", "vec3", "vec4",
        "int", "float",
        "sin", "cos",
        "sampler2D", "texture", "uniform",
        "ViewportSize",
        "gl_FragCoord",
    };
};
#endif //TEXTCOMPLETION_H
