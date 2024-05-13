//
// Created by Scott Kirila on 2024/04/29.
//

// DEPENDS ON OPENGL (Renderer)

#ifndef SHADER_H
#define SHADER_H
#include <string>

class Shader {
public:
	explicit Shader(const char* glslVersion);
	~Shader();

	void CompileVertexShader();
	void CompileFragmentShader();
	void CompileShaderProgram();

	unsigned int m_VertexShader{};
	std::string m_VertexShaderSource;
	std::string m_FragmentShaderSource;
	unsigned int m_FragmentShader{};
	unsigned int m_ShaderProgram{};

private:
	const char* m_GlslVersion{};
};



#endif //SHADER_H
