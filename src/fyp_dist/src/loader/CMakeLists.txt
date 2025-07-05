if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_subdirectory(linux)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_subdirectory(windows)
endif()


