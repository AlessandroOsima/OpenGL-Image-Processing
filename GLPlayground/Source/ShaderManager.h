#pragma once
#include <map>
#include <functional>
#include "ShaderProgram.h"

class ShaderManager
{
public:

	bool CreateShader(const std::string & ShaderProgramName, const std::string & VertexShaderFilename, const std::string & FragmentShaderFilename, size_t & ShaderProgramID);
	
	inline ShaderProgram & GetShader(const std::string & ShaderProgramName)
	{
		std::size_t hash = std::hash<std::string>{}(ShaderProgramName);
		return Shaders[hash];
	}

	inline ShaderProgram & GetShader(size_t ShaderProgramID)
	{
		return Shaders[ShaderProgramID];
	}

	void UseProgram(size_t Program);

	static ShaderManager & GetShaderManager()
	{
		static ShaderManager Manager;
		return Manager;
	}

private:
	std::map<std::size_t, ShaderProgram> Shaders;
};

