//
// Created by Scott Kirila on 2024/04/29.
//

// DEPENDS ON OPENGL

#ifndef RENDERER_H
#define RENDERER_H

#include <imgui.h>

class Window;
class Shader;

struct Vector2D {
    Vector2D() : x(0), y(0) {}
    Vector2D(const int _x, const int _y) : x(_x), y(_y) {}

    int x;
    int y;
};

class Renderer {
public:
    Renderer(Window* window, Shader* shader);
    ~Renderer();

    void Render();
    void Draw() const;
    void PostRender(const int &x, const int &y) const;

    Shader* m_Shader;
    // int m_ViewportWidth{};
    // int m_ViewportHeight{};
    unsigned int m_TextureColorBuffer{};
    Vector2D m_ViewportSize{};
    Vector2D m_FramebufferSize{};

private:
    unsigned int m_VertexBuffer{};
    unsigned int m_ArrayBuffer{};
    unsigned int m_Framebuffer{};
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

    ImVec4 m_ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    Window* m_Window;
};



#endif //RENDERER_H
