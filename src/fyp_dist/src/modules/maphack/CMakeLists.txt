remove_definitions(-DBUILD_FYP_LIB)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_library(maphack SHARED
        maphack.cc
        ../../sigscan.cc
    )
    add_dependencies(maphack luajit)
    set_property(TARGET maphack PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(maphack PUBLIC
        "${PROJECT_BINARY_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src/modules/core"
        ../../
    )

add_library(foo SHARED IMPORTED)
set_property(TARGET foo PROPERTY IMPORTED_LOCATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/fyp_main.so)


target_link_libraries(maphack PRIVATE foo memutil luajit)
    
endif()
