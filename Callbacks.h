//
// Created by hetan on 2024/04/28.
//

#ifndef CALLBACKS_H
#define CALLBACKS_H

class ImGuiInputTextCallbackData;

class Callbacks {
public:
    static int TextResizeCallback(ImGuiInputTextCallbackData* data);

    static void GlfwErrorCallback(int error, const char* description);
};



#endif //CALLBACKS_H
