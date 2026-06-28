#pragma once
#include <string>
#include <print>
#include <format>
#include <ctime>
#include <source_location>
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
     inline void Out(const std::string& file_name, int line, const LogLevel& level, const std::format_string<Args...> msg, Args&&... args)
     {
          #ifndef _DEBUG
          if (level > LogLevel::Info)
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

          std::println("{:02}:{:02}:{:02} {}:{} {}{:<7} {}\033[0m", 
               local_time->tm_hour,
               local_time->tm_min,
               local_time->tm_sec,
               file_name,
               line,
               color,
               label,
               std::format(msg, std::forward<Args>(args)...)
          );
     }
}

#define LOG_FATAL(...) \
     ::logger::Out(__FILE_NAME__, __LINE__, ::logger::LogLevel::Fatal, __VA_ARGS__)

#define LOG_ERROR(...) \
     ::logger::Out(__FILE_NAME__, __LINE__, ::logger::LogLevel::Error, __VA_ARGS__)

#define LOG_WARN(...) \
     ::logger::Out(__FILE_NAME__, __LINE__, ::logger::LogLevel::Warn, __VA_ARGS__)

#define LOG_INFO(...) \
     ::logger::Out(__FILE_NAME__, __LINE__, ::logger::LogLevel::Info, __VA_ARGS__)

#define LOG_DEBUG(...) \
     ::logger::Out(__FILE_NAME__, __LINE__, ::logger::LogLevel::Debug, __VA_ARGS__)

#define LOG_TRACE(...) \
     ::logger::Out(__FILE_NAME__, __LINE__, ::logger::LogLevel::Trace, __VA_ARGS__)

inline constexpr std::string VkResultToString(VkResult result)
{
    switch (result)
    {
    case VK_SUCCESS: return "VK_SUCCESS";
    case VK_NOT_READY: return "VK_NOT_READY";
    case VK_TIMEOUT: return "VK_TIMEOUT";
    case VK_EVENT_SET: return "VK_EVENT_SET";
    case VK_EVENT_RESET: return "VK_EVENT_RESET";
    case VK_INCOMPLETE: return "VK_INCOMPLETE";

    case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
    case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
    case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED";
    case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
    case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED";
    case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT";
    case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT";
    case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT";
    case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER";
    case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS";
    case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED";

    default:
        return "UNKNOWN_VK_RESULT";
    }
}

#define VK_CHECK(expr, ...) do {                                      \
     if (expr != VK_SUCCESS)                                        \
     {                                                                \
          LOG_ERROR("{} failed: {}", #expr, VkResultToString(expr));  \
     }                                                                \
} while (false)
