//
// Created by hetan on 5/28/2024.
//

#ifndef TEXTCOMPLETION_H
#define TEXTCOMPLETION_H
#include <list>
#include <string>
#include <vector>

class TextCompletion {
public:
    std::string GetCurrentWord(const ImGuiInputTextCallbackData* CallbackData);
    void PopulateMatches(const ImGuiInputTextCallbackData* CallbackData);
    void DisplayMatches(ImGuiInputTextCallbackData* CallbackData);
    void DoComplete(ImGuiInputTextCallbackData* data);
    void ClearResults();

    std::vector<std::string> m_Matches{};

    bool m_CanComplete{};
    int m_CurrentIndex{};
    char* m_CurrentWordStart{};
    char* m_CurrentWordEnd{};
    std::string m_SelectedMatch{};

    int m_WordStart{};

    bool m_DoComplete{};

private:
    void CalcListPos(const ImGuiInputTextCallbackData* CallbackData);

    float m_XPos{};
    float m_YPos{};

    const std::list<std::string> m_Dictionary = {
        "vec2", "vec3", "vec4",
        "int", "float",
        "sin", "cos",
        "sampler2D", "texture", "uniform",
        "ViewportSize",
        "gl_FragCoord",
        "Supercalifragilisticexpialidocious"
    };
};
#endif //TEXTCOMPLETION_H
