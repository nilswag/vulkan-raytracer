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
    void add_validation_layer(const char* layer_name);
    void enable_validation_layers();

    VkInstance instance = VK_NULL_HANDLE;
    std::vector<const char*> validation_layers;
};