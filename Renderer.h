//
// Created by Scott Kirila on 2024/04/29.
//

// DEPENDS ON OPENGL

#ifndef RENDERER_H
#define RENDERER_H

#include <imgui.h>

class Window;

class Renderer {
public:
    Renderer(Window* window);
    ~Renderer();

    Window* m_Window;
    unsigned int m_VertexBuffer{};
    unsigned int m_ArrayBuffer{};
    unsigned int m_FrameBuffer{};
    unsigned int m_TextureColorBuffer{};
    unsigned int m_RenderBuffer{};
    float m_Vertices[30] = {
        // positions         // texCoords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };
    ImVec2 m_FrameBufferSize;
};



#endif //RENDERER_H
