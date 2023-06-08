#pragma once 

#include <string>

struct ShaderSources {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	unsigned int m_RendererID;
	
public:
	std::string m_FilePath;

	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderSources ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& Source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentSHader);
	int GetUniformLocation(const std::string& name);
};