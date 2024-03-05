project('glimmer', 'c')

cmake = import('cmake')

# Dependencies
vulkan_dep = dependency('vulkan', version: '>=1.3')

# Subprojects
## dev dependencies
glfw_proj = cmake.subproject('glfw')
glfw_dep = glfw_proj.dependency('glfw')

glimmer_includedir = include_directories('include')

subdir('src')
subdir('test')
