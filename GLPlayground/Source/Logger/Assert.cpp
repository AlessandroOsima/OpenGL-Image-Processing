#include "stdafx.h"
#include "Asserts.h"
#include "Logger/Logger.h"
#include <assert.h>

void FailWithMessage(const std::string & Messsage)
{
	Logger::GetLogger().LogString(Messsage, LogType::ERROR);
	assert(nullptr);
}
