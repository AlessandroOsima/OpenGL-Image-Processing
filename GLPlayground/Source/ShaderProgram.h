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
	ShaderProgram(ShaderProgram && Program);
	void operator=(ShaderProgram && Program);

	//ShaderProgram & operator=(const ShaderProgram & Program) = default;

	inline unsigned int GetShaderProgramID() const
	{
		return ProgramID;
	}
	~ShaderProgram();

	bool CompileShader(const std::string & ShaderFilename, ShaderType Type);
	void LinkProgram();
	void UseProgram();
	unsigned int GetUniformBlockIndex(const std::string & BlockName);


private:
	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	GLuint ProgramID;
	bool Usable = false;
};

