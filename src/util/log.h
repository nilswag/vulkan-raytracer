#pragma once
#include <string>
#include <print>
#include <format>

namespace logger
{
     enum class LogLevel
     {
          FATAL,
          ERROR,
          WARN,
          INFO,
          DEBUG,
          TRACE
     };

     template<typename... Args>
     inline void out(const LogLevel& level, const std::format_string<Args...> msg, Args&&... args)
     {
          std::string prefix = "";
          switch (level)
          {
               case LogLevel::FATAL: prefix = "\033[0;31m[FATAL]"; break;
               case LogLevel::ERROR: prefix = "\033[0;31m[ERROR]"; break;
               case LogLevel::WARN:  prefix = "\033[0;33m[WARN]";  break;
               case LogLevel::INFO:  prefix = "\033[0;37m[INFO]";  break;
               case LogLevel::DEBUG: prefix = "\033[0;36m[DEBUG]"; break;
               case LogLevel::TRACE: prefix = "\033[0;35m[TRACE]"; break;
          }

          std::println("{} {} {}\033[0m", __TIME__, prefix, std::format(msg, std::forward<Args>(args)...));
     }

     template<typename... Args>
     inline void fatal(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::FATAL, msg, args...);
          throw std::exception();
     }

     template<typename... Args>
     inline void error(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::ERROR, msg, args...);
     }

     template<typename... Args>
     inline void warn(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::WARN, msg, args...);
     }

     template<typename... Args>
     inline void info(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::INFO, msg, args...);
     }

     template<typename... Args>
     inline void debug(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::DEBUG, msg, args...);
     }

     template<typename... Args>
     inline void trace(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::TRACE, msg, args...);
     }
}