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
    void PopulateMatches(const ImGuiInputTextCallbackData* CallbackData);

    std::vector<std::string> Matches{};

private:
    std::string GetCurrentWord(const ImGuiInputTextCallbackData* CallbackData);
    void ClearResults();

    bool canInitCursorPos{true};
    bool canUpdateCursorPos{true};
    int lastCursorPos{-1};

    char* CurrentWordStart{};
    char* CurrentWordEnd{};

    const std::list<std::string> Dictionary = {
        "vec2", "vec3", "vec4",
        "int", "float",
        "sin", "cos",
        "sampler2D", "texture", "uniform",
        "ViewportSize",
        "gl_FragCoord",
    };
};



#endif //TEXTCOMPLETION_H
