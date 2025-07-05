add_library(memutil INTERFACE)
target_include_directories(memutil PUBLIC INTERFACE .)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_include_directories(memutil PUBLIC INTERFACE linux linux/detail)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_include_directories(memutil PUBLIC INTERFACE windows windows/detail)
endif()


