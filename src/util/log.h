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
          Fatal,
          Error,
          Warn,
          Info,
          Debug,
          Trace
     };

     template<typename... Args>
     inline void Out(const LogLevel& level, const std::format_string<Args...> msg, Args&&... args)
     {
          #ifndef _DEBUG
          if (level > LogLevel::INFO)
               return;
          #endif

          std::string color;
          std::string label;
          switch (level)
          {
               case LogLevel::Fatal: color = "\033[0;31m"; label = "[FATAL]"; break;
               case LogLevel::Error: color = "\033[0;31m"; label = "[ERROR]"; break;
               case LogLevel::Warn:  color = "\033[0;33m"; label = "[WARN]";  break;
               case LogLevel::Info:  color = "\033[0;37m"; label = "[INFO]";  break;
               case LogLevel::Debug: color = "\033[0;36m"; label = "[DEBUG]"; break;
               case LogLevel::Trace: color = "\033[0;35m"; label = "[TRACE]"; break;
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
     inline void Fatal(const std::format_string<Args...> msg, Args&&... args)
     {
          Out(LogLevel::Fatal, msg, std::forward<Args>(args)...);
          throw std::exception();
     }

     template<typename... Args>
     inline void Error(const std::format_string<Args...> msg, Args&&... args)
     {
          Out(LogLevel::Error, msg, std::forward<Args>(args)...);
     }

     template<typename... Args>
     inline void Warn(const std::format_string<Args...> msg, Args&&... args)
     {
          Out(LogLevel::Warn, msg, std::forward<Args>(args)...);
     }

     template<typename... Args>
     inline void Info(const std::format_string<Args...> msg, Args&&... args)
     {
          Out(LogLevel::Info, msg, std::forward<Args>(args)...);
     }

     template<typename... Args>
     inline void Debug(const std::format_string<Args...> msg, Args&&... args)
     {
          Out(LogLevel::Debug, msg, std::forward<Args>(args)...);
     }

     template<typename... Args>
     inline void Trace(const std::format_string<Args...> msg, Args&&... args)
     {
          Out(LogLevel::Trace, msg, std::forward<Args>(args)...);
     }
}

static inline void VkCheck(const VkResult& result, const std::string& prefix, const std::string& func_name)
{
     if (result < VK_SUCCESS)
          logger::Error("{}: {} failed", prefix, func_name);
}