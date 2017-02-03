#pragma once
#include <string>


#ifdef _DEBUG
#define FailWithMessage(string) {\
	Logger::GetLogger().LogString(string, LogType::ERROR); \
	assert(nullptr); \
} \

#define AssertWithMessage(condition, string) {\
	if (!(condition)) \
	{ \
		Logger::GetLogger().LogString(string, LogType::ERROR); \
		assert(nullptr); \
	} \
} \

#else

#define FailWithMessage(string) {\
} \

#define AssertWithMessage(condition, string) {\
} \

#endif