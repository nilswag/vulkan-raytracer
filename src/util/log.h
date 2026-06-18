#pragma once
#include <string>
#include <print>

namespace log
{
     enum class LogLevel
     {
          FATAL,
          ERROR,
          WARN,
          INFO,
          DEBUG,
          TRACE
     }

     template<typename T, Args... args>
     inline void log(LogLevel& level, const std::string& fmt, Args... args)
     {
          
     }

     inline void fatal()
     {

     }

     inline void error()
     {

     }

     inline void warn()
     {

     }

     inline void info()
     {

     }

     inline void debug()
     {

     }

     inline void trace()
     {
          
     }
}