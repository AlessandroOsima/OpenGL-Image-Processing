#pragma once
#include "Logger/Logger.h"
#include <GL/glew.h>
#include <sstream>

#define GL_DEBUG 1

#ifdef GL_DEBUG
#define glCheckError(functionName)  { \
  	GLuint res = glGetError(); \
	if(res != GL_NO_ERROR) \
    { \
		const GLubyte * errorString = gluErrorString(res); \
		std::stringstream stream; \
		stream << "GL error with string code : " << (const char *)errorString << " in function : " << functionName << std::ends; \
		Logger::GetLogger().LogString(stream.str(), LogType::ERROR); \
    } \
} \

#define glCheckFunction(function)  { \
	function; \
  	glCheckError(#function) \
} \

#else

#define glCheckError(functionName) {}

#define glCheckFunction(function)  { \
	function; \
}\

#endif

