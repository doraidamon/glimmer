#include "vulkan_app.h"

#include <assert.h>

int main()
{
    VulkanApp app = {0};
    assert(init_app(&app));
    return 0;
}
