# TODO: This has an aweful lot of code duplication.

add_definitions(-DBUILD_FYP_LIB)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_library(main SHARED
        main.cc
        core.cc
        module_loader.cc
        ../../sigscan.cc
    )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_library(main SHARED
        main.cc
        core.cc
        module_loader.cc
        ../../memutil/windows/detail/libpe.cc
        ../../sigscan.cc
    )
endif()

set_property(TARGET main PROPERTY POSITION_INDEPENDENT_CODE ON)

add_dependencies(main luajit memutil funchook_src)
target_include_directories(main PUBLIC
    "${PROJECT_BINARY_DIR}/include"
    ../../
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_include_directories(main PUBLIC linux)
    target_link_libraries(main
        luajit
        memutil
        funchook-static
        funchook-distorm
        ${CMAKE_DL_LIBS}
    )

endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_include_directories(main PUBLIC windows)
    set_target_properties(main PROPERTIES LINK_FLAGS "-static")
    target_link_libraries(main
        luajit
        memutil
        funchook-static
        funchook-distorm
        ${CMAKE_DL_LIBS}
        -lpsapi
        -lws2_32
    )

endif()

