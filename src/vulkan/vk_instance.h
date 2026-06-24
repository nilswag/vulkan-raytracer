#pragma once
#include <vector>
#include <vulkan/vulkan.h>

struct AppInfo;

class Instance
{
public:
    void init(const AppInfo& app_info);
    ~Instance();

    inline VkInstance get() const { return instance; }

private:
    void create_debug_messenger();
    void destroy_debug_messenger();
    void validate_layers();
    void validate_extensions();

    std::vector<const char*> requested_layers;
    std::vector<const char*> requested_extensions;
    VkInstance instance;
    
    VkDebugUtilsMessengerEXT debug_messenger;
};