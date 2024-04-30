//
// Created by Scott Kirila on 2024/04/29.
//

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

private:
	const char* m_GlslVersion{};
	unsigned int m_VertexShader{};
	unsigned int m_FragmentShader{};
	unsigned int m_ShaderProgram{};

	std::string m_VertexShaderSource;
	std::string m_FragmentShaderSource;
};



#endif //SHADER_H
