#include "ShaderManager.h"
#include "Logger/Logger.h"
#include "Managers/ResourceManager.h"

bool ShaderManager::CreateShader(const std::string & ShaderProgramName, const std::string & VertexShaderFilename, const std::string & FragmentShaderFilename, size_t & ShaderProgramID)
{
	ShaderProgram prg;

	//FIXME : Generating a string for each resource load is TERRIBLE
	std::string vsStr = ResourceManager::GetShadersForlder();

	if (!prg.CompileShader(vsStr += VertexShaderFilename, ShaderType::Vertex))
	{
		Logger::GetLogger().LogString("Vertex shader compile failed", ERROR);
		return false;
	}
	std::string fsStr = ResourceManager::GetShadersForlder();

	if (!prg.CompileShader(fsStr += FragmentShaderFilename, ShaderType::Fragment))
	{
		Logger::GetLogger().LogString("Fragment shader compile failed", ERROR);
		return false;
	}

	prg.LinkProgram();

	std::size_t hash = std::hash<std::string>{}(ShaderProgramName);
	ShaderProgramID = hash;

	Shaders[hash] = std::move(prg);

	if (OnShaderAdded)
	{
		OnShaderAdded(hash);
	}

	return true;
}

ShaderProgram & ShaderManager::GetShader(size_t ShaderProgramID, bool & FoundShader)
{
	FoundShader = true;
	std::map<std::size_t, ShaderProgram>::iterator it = Shaders.find(ShaderProgramID);

	if (it == Shaders.end())
	{
		FoundShader = false;
	}

	return  Shaders[ShaderProgramID];
}

void ShaderManager::UseProgram(size_t Program)
{
	std::map<std::size_t, ShaderProgram>::iterator it = Shaders.find(Program);

	if (it != Shaders.end())
	{
		it->second.UseProgram();
	}
}
