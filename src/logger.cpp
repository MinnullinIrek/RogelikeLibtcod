#include "logger.h"

#include <stdio.h>

#include <cstdarg>
#include <fstream>
#include <string>

 void Logger::log(std::string_view filename, std::string_view function, int line, const char* format, ...){
 char* sMessage = nullptr;
 auto nLength = 0u;
 va_list args;
 va_start(args, format);
 auto sLine = std::to_string(line);
 nLength = _vscprintf(format, args) + 1 + filename.length() + function.length() + sLine.length();
 sMessage = new char[nLength];
 vsprintf_s(sMessage, nLength, format, args);

 printf(sMessage);

 delete[] sMessage;
 };
