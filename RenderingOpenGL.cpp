//
// Created by Scott Kirila on 2024/04/29.
//

#include <iostream>

#include "glad/glad.h"

#include "RenderingOpenGL.h"
#include "Shader.h"
#include "Window.h"

using namespace Rendering;

OpenGL::OpenGL(Window* window, Shader* shader)
: Base(window, shader) {

    glGenVertexArrays(1, &m_ArrayBuffer);
    glGenBuffers(1, &m_VertexBuffer);
    glBindVertexArray(m_ArrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), static_cast<void *>(nullptr));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Framebuffer
    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

    // Framebuffer color attachment
    glGenTextures(1, &m_TextureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
        m_FramebufferSize.x, m_FramebufferSize.y,
        0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        m_TextureColorBuffer, 0);

    glGenRenderbuffers(1, &m_RenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
        m_FramebufferSize.x, m_FramebufferSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER, m_RenderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR: Framebuffer is incomplete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGL::~OpenGL() {
    std::cout << "Renderer\n";
    glBindBuffer(GL_RENDERBUFFER, 0);
    glBindBuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OpenGL::Render() {
    if (m_FramebufferSize.x != m_ViewportSize.x || m_FramebufferSize.y != m_ViewportSize.y) {
        m_FramebufferSize.x = m_ViewportSize.x;
        m_FramebufferSize.y = m_ViewportSize.y;

        glDeleteFramebuffers(1, &m_Framebuffer);
        glDeleteRenderbuffers(1, &m_RenderBuffer);

        glGenFramebuffers(1, &m_Framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

        glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);

        glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_FramebufferSize.x, m_FramebufferSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorBuffer, 0);

        glGenRenderbuffers(1, &m_RenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_FramebufferSize.x, m_FramebufferSize.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
        glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
    }
}

void OpenGL::Draw() const {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(m_Shader->m_ShaderProgram);

    const int u_ViewportSize = glGetUniformLocation(m_Shader->m_ShaderProgram, "ViewportSize");
    glUniform2i(u_ViewportSize, m_ViewportSize.x, m_ViewportSize.y);

    const int u_Time = glGetUniformLocation(m_Shader->m_ShaderProgram, "Time");
    glUniform1f(u_Time, static_cast<float>(Window::GetTime()));

    glBindVertexArray(m_ArrayBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGL::PostRender(const int &x, const int &y) const {

    glViewport(0, 0, x, y);
    glClearColor(m_ClearColor.x * m_ClearColor.w, m_ClearColor.y * m_ClearColor.w, m_ClearColor.z * m_ClearColor.w, m_ClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
}
