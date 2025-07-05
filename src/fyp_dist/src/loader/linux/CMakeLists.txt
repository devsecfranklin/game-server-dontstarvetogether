set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
set(CMAKE_C_FLAGS "-m64 -Wl,--no-as-needed -shared -fpic -ldl")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")
set(CMAKE_SHARED_LIBRARY_C_FLAGS "")
add_library(loader SHARED
    loader.cc
    ../../sigscan.cc
)

add_library(shim_curl SHARED
    shim_curl.c)

add_dependencies(loader memutil funchook_src)
target_include_directories(loader PUBLIC
    ${PROJECT_BINARY_DIR}/include
    funchook_src
    ../../
)
target_link_libraries(loader
    ${CMAKE_DL_LIBS}
    memutil
    funchook-static
    funchook-distorm
)

target_link_libraries(shim_curl)
