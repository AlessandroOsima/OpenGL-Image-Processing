#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
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

	unsigned int GetUniformBufferBlockIndex(const std::string & BlockName);
	unsigned int GetUniformIndex(const std::string & UniformName);

	void SetUniformMatrix4(unsigned int Location,  const glm::mat4 & Mat4Val);
	void SetUniformMatrix3(unsigned int Location, const glm::mat3 & Mat3Val);
	void SetUniformVector4(unsigned int Location, const glm::vec4 & Vec4Val);
	void SetUniformVector3(unsigned int Location, const glm::vec3 & Vec3Val);
	void SetUniformFloat(unsigned int Location, float FloatVal);

private:
	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	GLuint ProgramID;
	bool Usable = false;
};

