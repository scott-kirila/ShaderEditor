//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef RENDERER_H
#define RENDERER_H



class Renderer {
public:
    Renderer();
    ~Renderer();

    unsigned int m_VertexBuffer{};
    unsigned int m_ArrayBuffer{};
    unsigned int m_FrameBuffer{};
    unsigned int m_TextureColorBuffer{};
    unsigned int m_RenderBuffer{};
};



#endif //RENDERER_H
