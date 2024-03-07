///
/// A testing framework for vulkan driver on glimmer.
///

#ifndef _VULKAN_APP_H_
#define _VULKAN_APP_H_

#include "vulkan/vulkan_core.h"
#define error(s) fprintf(stderr, "%s\n", s);

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

const char* APP_NAME = "vkapp";

const char* LAYERS[] = { "VK_LAYER_KHRONOS_validation" };

/* contains an instance, devices and more needed for rendering. */
typedef struct
{
    VkInstance instance;
    VkPhysicalDevice gpu;
    VkDevice device;
    VkCommandPool command_pool;
} VulkanApp;

bool create_instance(VulkanApp* app, const char** extensions, uint32_t count)
{
    VkResult res;

    VkApplicationInfo app_info = { 0 };
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = APP_NAME;
    app_info.pEngineName = APP_NAME;
    app_info.apiVersion = VK_VERSION_1_2;
    app_info.pNext = NULL;

    const uint32_t layers_length = sizeof(LAYERS) / sizeof(LAYERS[0]);

    VkInstanceCreateInfo instance_info = { 0 };
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.enabledLayerCount = layers_length;
    instance_info.ppEnabledLayerNames = LAYERS;
    instance_info.ppEnabledExtensionNames = extensions;
    instance_info.enabledExtensionCount = count;
    instance_info.pApplicationInfo = &app_info;

    res = vkCreateInstance(&instance_info, NULL, &app->instance);

    if (res)
    {
        return false;
    }

    return true;
}

bool create_device(VulkanApp* app)
{
    VkResult res;

    uint32_t gpu_count = 0;
    res = vkEnumeratePhysicalDevices(app->instance, &gpu_count, NULL);
    if (res)
    {
        return false;
    }

    VkPhysicalDevice* gpus = malloc(sizeof(VkPhysicalDevice) * gpu_count);
    res = vkEnumeratePhysicalDevices(app->instance, &gpu_count, gpus);
    if (res)
    {
        return false;
    }

    /* Select device 0 */
    app->gpu = gpus[0];

    /* get queue index */
    uint32_t prop_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(app->gpu, &prop_count, NULL);

    VkQueueFamilyProperties* props = malloc(sizeof(VkQueueFamilyProperties) * prop_count);
    vkGetPhysicalDeviceQueueFamilyProperties(app->gpu, &prop_count, props);

    uint32_t graphics_queue = ~0u;
    for (uint32_t i = 0; i < prop_count; i++)
    {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphics_queue = i;
            break;
        }
    }

    const float default_queue_priority = 1.0f;

    VkDeviceQueueCreateInfo device_queue_info = { 0 };
    device_queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    device_queue_info.queueFamilyIndex = graphics_queue;
    device_queue_info.queueCount = 1;
    device_queue_info.pQueuePriorities = &default_queue_priority;

    uint32_t devext_prop_count = 0;
    vkEnumerateDeviceExtensionProperties(app->gpu, NULL, &devext_prop_count, NULL);

    VkExtensionProperties* devext_props = malloc(sizeof(VkExtensionProperties) * devext_prop_count);
    vkEnumerateDeviceExtensionProperties(app->gpu, NULL, &devext_prop_count, devext_props);

    const char** devexts = malloc(sizeof(const char*) * devext_prop_count);

    for (uint32_t i = 0; i < devext_prop_count; i++)
    {
        devexts[i] = devext_props[i].extensionName;
    }

    VkDeviceCreateInfo device_info = { 0 };
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.ppEnabledExtensionNames = devexts;
    device_info.enabledExtensionCount = devext_prop_count;
    device_info.pQueueCreateInfos = &device_queue_info;
    device_info.queueCreateInfoCount = 1;

    vkCreateDevice(app->gpu, &device_info, NULL, &app->device);

    /* create command pool */
    VkCommandPoolCreateInfo cpool_info = { 0 };
    cpool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cpool_info.queueFamilyIndex = graphics_queue;
    cpool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    vkCreateCommandPool(app->device, &cpool_info, NULL, &app->command_pool);

    return true;
}

bool init_app(VulkanApp* app)
{
    memset(app, 0, sizeof(VulkanApp));

    if (!glfwInit())
    {
        error("unable to init glfw");
        return false;
    }

    if (!glfwVulkanSupported())
    {
        error("vulkan is not supported");
        return false;
    }

    uint32_t count;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    if (!create_instance(app, extensions, count))
    {
        error("failed to create vulkan instance");
        return false;
    }

    if (!create_device(app))
    {
        error("failed to create vulkan device");
        return false;
    }

    return true;
}

#endif
