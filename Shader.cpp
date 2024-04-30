//
// Created by Scott Kirila on 2024/04/29.
//

#include "Shader.h"

#include <glad/glad.h>
#include <iostream>

std::string vertexShaderSource =
R"(
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos, 1);
	TexCoords = aTexCoords;
})";

std::string fragmentShaderSource =
R"(
in vec2 TexCoords;
out vec4 FragColor;

uniform vec2 ViewportSize;
uniform float Time;

void main()
{
	FragColor = vec4(vec3(0), 1);
})";

Shader::Shader(const char* glslVersion) : m_GlslVersion(glslVersion) {
	m_VertexShaderSource = m_GlslVersion + vertexShaderSource;
	m_FragmentShaderSource = m_GlslVersion + fragmentShaderSource;

	CompileShaderProgram();
}

Shader::~Shader() {
    glDeleteProgram(m_ShaderProgram);
    glDeleteShader(m_FragmentShader);
    glDeleteShader(m_VertexShader);
}

void Shader::CompileVertexShader() {
    m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* source = m_VertexShaderSource.c_str();
    glShaderSource(m_VertexShader, 1, &source, nullptr);
    glCompileShader(m_VertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_VertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void Shader::CompileFragmentShader() {
    m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = m_FragmentShaderSource.c_str();
    glShaderSource(m_FragmentShader, 1, &source, nullptr);
    glCompileShader(m_FragmentShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_FragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}




void Shader::CompileShaderProgram() {
	CompileVertexShader();
	CompileFragmentShader();

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, m_VertexShader);
	glAttachShader(m_ShaderProgram, m_FragmentShader);
	glLinkProgram(m_ShaderProgram);
	// check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// glDeleteShader(vertexShader);
	// glDeleteShader(fragmentShader);
}
