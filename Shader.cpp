//
// Created by Scott Kirila on 2024/04/29.
//

#include "Shader.h"

#include <glad/glad.h>

Shader::Shader(const char *glslVersion) {
}

Shader::~Shader() {
    glDeleteProgram(m_ShaderProgram);
    glDeleteShader(m_FragmentShader);
    glDeleteShader(m_VertexShader);
}

void Shader::CompileShader() {
}

void Shader::CompileShaderProgram() {
}
