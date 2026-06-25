#pragma once
#include <string>
#include <print>
#include <format>
#include <ctime>
#include <vulkan/vulkan.h>

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
          #ifndef _DEBUG
          if (level > LogLevel::INFO)
               return;
          #endif

          std::string color;
          std::string label;
          switch (level)
          {
               case LogLevel::FATAL: color = "\033[0;31m"; label = "[FATAL]"; break;
               case LogLevel::ERROR: color = "\033[0;31m"; label = "[ERROR]"; break;
               case LogLevel::WARN:  color = "\033[0;33m"; label = "[WARN]";  break;
               case LogLevel::INFO:  color = "\033[0;37m"; label = "[INFO]";  break;
               case LogLevel::DEBUG: color = "\033[0;36m"; label = "[DEBUG]"; break;
               case LogLevel::TRACE: color = "\033[0;35m"; label = "[TRACE]"; break;
          }
          
          const std::time_t now = std::time(nullptr);
          
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif
          const std::tm* local_time = std::localtime(&now);
#ifdef _MSC_VER
#pragma warning(pop)
#endif

          std::println("{:02}:{:02}:{:02} {}{:<7} {}\033[0m", 
               local_time->tm_hour,
               local_time->tm_min,
               local_time->tm_sec,
               color,
               label, 
               std::format(msg, std::forward<Args>(args)...)
          );
     }

     template<typename... Args>
     inline void fatal(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::FATAL, msg, std::forward<Args>(args)...);
          throw std::exception();
     }

     template<typename... Args>
     inline void error(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::ERROR, msg, std::forward<Args>(args)...);
     }

     template<typename... Args>
     inline void warn(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::WARN, msg, std::forward<Args>(args)...);
     }

     template<typename... Args>
     inline void info(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::INFO, msg, std::forward<Args>(args)...);
     }

     template<typename... Args>
     inline void debug(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::DEBUG, msg, std::forward<Args>(args)...);
     }

     template<typename... Args>
     inline void trace(const std::format_string<Args...> msg, Args&&... args)
     {
          out(LogLevel::TRACE, msg, std::forward<Args>(args)...);
     }
}

static inline void chk(const VkResult& result, const std::string& prefix, const std::string& func_name)
{
     if (result < VK_SUCCESS)
          logger::error("{}: {} failed", prefix, func_name);
}