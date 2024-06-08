//
// Created by Scott Kirila on 5/15/2024.
//

#ifndef RENDERERBASE_H
#define RENDERERBASE_H

#include "Vector.h"
#include "Window.h"

namespace Rendering {

    class Base {
    public:
        Base(Window* window, const char* shaderVersion);

        virtual ~Base();

        virtual void Render() = 0;
        virtual void Draw() const = 0;
        virtual void PostRender(const int &x, const int &y) const = 0;

        const char* m_ShaderVersion;
        Vector2Di m_ViewportSize{};
        Vector2Di m_FramebufferSize{};

    protected:
        Window* m_Window;

        Vector4Df m_ClearColor {0.45f, 0.55f, 0.60f, 1.00f};

        float m_Vertices[30] = {
            // positions         // texCoords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f
        };
    };

}

#endif //RENDERERBASE_H
