//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
public:
    explicit Shader(const char *glslVersion);

    ~Shader();

    void CompileShaderProgram();

    std::string m_VertexShaderSource;
    std::string m_FragmentShaderSource;
    unsigned int m_ShaderProgram{};

private:
    void CompileVertexShader();

    void CompileFragmentShader();

    unsigned int m_VertexShader{};
    unsigned int m_FragmentShader{};
    const char *m_GlslVersion{};
};

#endif //SHADER_H
