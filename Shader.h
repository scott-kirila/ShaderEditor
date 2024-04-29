//
// Created by Scott Kirila on 2024/04/29.
//

#ifndef SHADER_H
#define SHADER_H



class Shader {
public:
	explicit Shader(const char* glslVersion);
	~Shader();

	void CompileShader();
	void CompileShaderProgram();

private:
	const char* m_GlslVersion;
	unsigned int m_VertexShader;
	unsigned int m_FragmentShader;
	unsigned int m_ShaderProgram;

	const char* vertexShaderSource =
	R"(#version 330 core
		layout (location = 0) in vec3 aPos;
	    layout (location = 1) in vec2 aTexCoords;
	    out vec2 TexCoords;
		void main()
		{
		   gl_Position = vec4(aPos, 1);
	       TexCoords = aTexCoords;
		})";

	const char* fragmentShaderSource =
	R"(#version 330 core
    in vec2 TexCoords;
	out vec4 FragColor;

    uniform vec2 ViewportSize;
    uniform float Time;

	void main()
	{
		FragColor = vec4(vec3(0), 1);
	})";
};



#endif //SHADER_H
