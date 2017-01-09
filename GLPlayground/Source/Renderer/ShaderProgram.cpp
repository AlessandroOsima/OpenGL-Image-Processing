#include "ShaderProgram.h"
#include "Renderer/GLUtilities.h"
#include <strstream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <assert.h>
#include "Logger/Logger.h"
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram()
{
	ProgramID = glCreateProgram();
}


ShaderProgram::ShaderProgram(ShaderProgram && Program) : 
	VertexShaderID(std::move(Program.VertexShaderID)),
	FragmentShaderID(std::move(Program.FragmentShaderID)),
	ProgramID(std::move(Program.ProgramID)),
	Usable(std::move(Program.Usable))
{
	//Invalidate the other program to avoid shader deletion on destruction
	Program.Usable = false;
}

void ShaderProgram::operator=(ShaderProgram && Program)
{
	VertexShaderID = std::move(Program.VertexShaderID);
	FragmentShaderID = std::move(Program.FragmentShaderID);
	ProgramID = std::move(Program.ProgramID);
	Usable = std::move(Program.Usable);

	//Invalidate the other program to avoid shader deletion on destruction
	Program.Usable = false;
}

ShaderProgram::~ShaderProgram()
{
	if (Usable)
	{
		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);
		glDeleteProgram(ProgramID);
	}
}

bool ShaderProgram::BindBufferToUniform(uint32_t BufferID, uint32_t BindingLocation, const char * BindingName)
{
	unsigned int shaderUniformID = GetUniformBufferBlockIndex(BindingName);

	if (shaderUniformID != GL_INVALID_INDEX)
	{
		glUniformBlockBinding(GetShaderProgramID(), shaderUniformID, BindingLocation);

		glBindBufferBase(GL_UNIFORM_BUFFER, BindingLocation, BufferID);

		return true;
	}

	return false;
}

bool ShaderProgram::CompileShader(const std::string & ShaderFilename, ShaderType Type)
{

	std::fstream fileStream;

	fileStream.open(ShaderFilename);

	if (!fileStream.is_open())
	{
		std::stringstream stream;
		stream << "Unable to open shader file " << ShaderFilename << std::ends;
		Logger::GetLogger().LogString(stream.str(), LogType::ERROR);

		return false;
	}

	std::stringstream stream;
	stream << fileStream.rdbuf();

	std::string fc((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

	const char * fileContent = fc.c_str();

	GLint compilationSuccess = 0;
	GLuint CompiledID = 0;

	switch (Type)
	{
	case ShaderType::Vertex:
	{
		CompiledID = VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShaderID, 1, &fileContent, NULL);
		glCompileShader(VertexShaderID);
		break;
	}
	case ShaderType::Fragment:
	{
		CompiledID = FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShaderID, 1, &fileContent, NULL);
		glCompileShader(FragmentShaderID);

		break;
	}

	default:
		break;
	}

	glGetShaderiv(CompiledID, GL_COMPILE_STATUS, &compilationSuccess);

	GLint logSize;
	glGetShaderiv(CompiledID, GL_INFO_LOG_LENGTH, &logSize);

	if (logSize)
	{
		char * logMessage = new char[logSize];

		glGetShaderInfoLog(CompiledID, logSize, NULL, logMessage);

		std::stringstream stream;
		stream << "Shader " << CompiledID << " compile log: \n" << logMessage << std::ends;
		Logger::GetLogger().LogString(stream.str(), ERROR);

		delete logMessage;
	}

	if (compilationSuccess)
	{
		glAttachShader(ProgramID, CompiledID);
	}

	Usable = compilationSuccess == 1;

	return Usable;
}

void ShaderProgram::LinkProgram()
{
	glCheckFunction(glLinkProgram(ProgramID));

	GLint linkSuccess;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &linkSuccess);

	std::stringstream stream;
	stream << "Shader " << ProgramID << " link result is : " << linkSuccess << std::ends;
	Logger::GetLogger().LogString(stream.str(), LOG);

	GLint logSize;
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &logSize);

	if (logSize)
	{
		char * logMessage = new char[logSize];

		glGetProgramInfoLog(ProgramID, logSize, NULL, logMessage);

		stream.clear();
		stream << "Shader " << ProgramID << " link log: \n" << logMessage << std::ends;
		Logger::GetLogger().LogString(stream.str(), LOG);

		delete logMessage;
	}
}

void ShaderProgram::UseProgram()
{
	glUseProgram(ProgramID);
}

unsigned int ShaderProgram::GetUniformBufferBlockIndex(const std::string & BlockName)
{
	return glGetUniformBlockIndex(ProgramID, BlockName.c_str());
}

unsigned int ShaderProgram::GetUniformIndex(const std::string & UniformName)
{
	return glGetUniformLocation(ProgramID, UniformName.c_str());
}

void ShaderProgram::SetUniformMatrix4(unsigned int Location, const glm::mat4 & Mat4Val)
{
	glUniformMatrix4fv(Location, 1, false, glm::value_ptr(Mat4Val));
}

void ShaderProgram::SetUniformMatrix3(unsigned int Location, const glm::mat3 & Mat3Val)
{
	glUniformMatrix3fv(Location, 1, false, glm::value_ptr(Mat3Val));
}

void ShaderProgram::SetUniformVector4(unsigned int Location, const glm::vec4 & Vec4Val)
{
	glUniform4f(Location, Vec4Val.x, Vec4Val.y, Vec4Val.z, Vec4Val.w);
}

void ShaderProgram::SetUniformVector3(unsigned int Location, const glm::vec3 & Vec3Val)
{
	glUniform3f(Location, Vec3Val.x, Vec3Val.y, Vec3Val.z);
}

void ShaderProgram::SetUniformFloat(unsigned int Location, float FloatVal)
{
	glUniform1f(Location, FloatVal);
}

