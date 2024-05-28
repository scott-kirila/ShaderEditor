//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "RenderingBase.h"
#include "Shader.h"

class Window;

namespace Rendering {

    class OpenGL final : public Base {
    public:
        OpenGL(Window* window, const char* shaderVersion);
        ~OpenGL() override;

        void Render() override;
        void Draw() const override;
        void PostRender(const int &x, const int &y) const override;

        std::unique_ptr<Shader> m_Shader;
        unsigned int m_TextureColorBuffer{};

    private:
        unsigned int m_VertexBuffer{};
        unsigned int m_ArrayBuffer{};
        unsigned int m_Framebuffer{};
        unsigned int m_RenderBuffer{};
    };

}

#endif //RENDERER_H
