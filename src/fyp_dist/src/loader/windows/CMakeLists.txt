add_library(winmm_loader MODULE
    winmm_loader.cc
    ../../memutil/windows/detail/libpe.cc
    ../../sigscan.cc
)
set_property(TARGET winmm_loader PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(winmm_loader PROPERTIES LINK_FLAGS "-static -Wl,--image-base -Wl,0x10000000")

add_dependencies(winmm_loader memutil funchook_src)

target_include_directories(winmm_loader PUBLIC
    ${PROJECT_BINARY_DIR}/include
    funchook_src
    ../../
)
target_link_libraries(winmm_loader
    memutil
    -lpsapi
    funchook-withdep
)
