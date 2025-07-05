cmake_minimum_required(VERSION 3.22.1)
include(ExternalProject)
include(cmake/subdirlist.cmake)

project(Fyp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Prefix Libraries with fyp_ for modloader and to ease identification
set(CMAKE_SHARED_LIBRARY_PREFIX fyp_)
set(CMAKE_SHARED_MODULE_PREFIX fyp_)

# Put outputs somewhere easy to get
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src/deps)
add_subdirectory(src/memutil)
add_subdirectory(src/loader)

subdirlist(MODULES ${CMAKE_SOURCE_DIR}/src/modules/)
foreach(subdir ${MODULES})
    message(Building subdir: ${subdir})
    add_subdirectory(src/modules/${subdir})
endforeach()
