glimmer_sources = [
  'glimmer.c',
]

glimmer_lib = static_library('glimmer', sources: glimmer_sources, include_directories: glimmer_includedir, dependencies: [vulkan_dep])

glimmer_dep = declare_dependency(link_with: glimmer_lib, include_directories: glimmer_includedir)
