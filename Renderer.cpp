//
// Created by Scott Kirila on 2024/04/29.
//

#include "Renderer.h"

#include <glad/glad.h>

Renderer::Renderer() {
}

Renderer::~Renderer() {
    glBindBuffer(GL_RENDERBUFFER, 0);
    glBindBuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
