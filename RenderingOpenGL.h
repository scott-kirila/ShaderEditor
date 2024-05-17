//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "RenderingBase.h"

class Window;
class Shader;

namespace Rendering {

    class OpenGL final : public Base {
    public:
        OpenGL(Window* window, Shader* shader);
        ~OpenGL() override;

        void Render() override;
        void Draw() const override;
        void PostRender(const int &x, const int &y) const override;

        unsigned int m_TextureColorBuffer{};

    private:
        unsigned int m_VertexBuffer{};
        unsigned int m_ArrayBuffer{};
        unsigned int m_Framebuffer{};
        unsigned int m_RenderBuffer{};
    };

}

#endif //RENDERER_H