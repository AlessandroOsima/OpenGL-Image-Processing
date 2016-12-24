#pragma once

#include <GL/glew.h>
#include <string>

enum  class ShaderType
{
	Vertex,
	Fragment
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	bool CompileShader(const std::string & ShaderFilename, ShaderType Type);
	void LinkProgram();
	void UseProgram();

private:
	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	GLuint ProgramID;
};

