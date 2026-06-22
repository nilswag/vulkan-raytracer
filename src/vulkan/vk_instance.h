#pragma once
#include <vector>
#include <vulkan/vulkan.h>

struct AppInfo;

class Instance
{
public:
    void init(const AppInfo& app_info);
    ~Instance();

private:
    void create_debug_messenger();
    void validate_layers();
    void validate_extension();

    void add_validation_layer(const char* layer_name);
    void check_validation_layers();

    std::vector<const char*> validation_layers;
    std::vector<const char*> extensions;
    VkInstance instance;
    
    VkDebugUtilsMessengerEXT debug_messenger;
};