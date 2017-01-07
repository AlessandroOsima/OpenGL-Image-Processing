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
	~ShaderProgram();
	
	inline unsigned int GetShaderProgramID() const
	{
		return ProgramID;
	}

	bool BindBufferToUniform(uint32_t BufferID, uint32_t BindingLocation, const char * BindingName);

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

