#include <iostream>
#include "Headers/Log.h"

void Log(const std::string& message)
{
	std::cout << message << std::endl;
}

void InitLog()
{
	Log("Initializing Logs\n...");
}

void ErrorLog(const std::string& message)
{
	std::cout << message << std::endl;
}