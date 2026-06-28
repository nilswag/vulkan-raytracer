#pragma once
#include <vector>
#include <vulkan/vulkan.h>

struct AppInfo;

class Instance
{
public:
    void Init(const AppInfo& app_info);
    ~Instance();

    inline VkInstance Get() const { return instance_; }

private:
    void CreateDebugMessenger();
    void DestroyDebugMessenger();
    void ValidateLayers();
    void ValidateExtensions();

    std::vector<const char*> requested_layers_;
    std::vector<const char*> requested_extensions_;
    VkInstance instance_;
    
    VkDebugUtilsMessengerEXT debug_messenger_;
};