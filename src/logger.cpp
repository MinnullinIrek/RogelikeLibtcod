#include "logger.h"

#include <stdio.h>

#include <cstdarg>
#include <fstream>
#include <string>

#ifndef win32
  #define _vscprintf vprintf

#endif

 void Logger::log(std::string_view filename, std::string_view function, int line, const char* format, ...){
 char* sMessage = nullptr;
 size_t nLength;
 va_list args;
 va_start(args, format);
 auto sLine = std::to_string(line);
 nLength = _vscprintf(format, args) + 1 + filename.length() + function.length() + sLine.length();
 sMessage = new char[nLength];
#ifdef win32
 vsprintf_s(sMessage, nLength, format, args);
#else
 vsprintf(sMessage, format, args);
 // nLength, format, args);
#endif
 printf("%s", sMessage);

 delete[] sMessage;
 };
