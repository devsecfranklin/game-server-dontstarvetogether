include(FetchContent)

ExternalProject_Add(
    funchook_src
    GIT_REPOSITORY https://github.com/kubo/funchook.git
    INSTALL_DIR ${CMAKE_BINARY_DIR}/
    BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/src/deps/funchook_src-prefix/src/funchook_src-build/libdistorm.a
                     ${CMAKE_BINARY_DIR}/lib/libfunchook.a
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
               -DCMAKE_INSTALL_LIBDIR=${CMAKE_BINARY_DIR}/lib
               -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true
               -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
               -DFUNCHOOK_DISASM=distorm
               -DFUNCHOOK_INSTALL=true
               -DFUNCHOOK_BUILD_TESTS=false
               -DFUNCHOOK_BUILD_STATIC=true
)

add_library(funchook-static STATIC UNKNOWN IMPORTED GLOBAL)
add_library(funchook-distorm STATIC UNKNOWN IMPORTED GLOBAL)
add_library(funchook-withdep INTERFACE IMPORTED GLOBAL)

set_target_properties(funchook-static PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/libfunchook.a
    INTERFACE_INCLUDE ${CMAKE_BINARY_DIR}/include/
)

set_target_properties(funchook-distorm PROPERTIES
    IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/src/deps/funchook_src-prefix/src/funchook_src-build/libdistorm.a
    INTERFACE_INCLUDE ${CMAKE_BINARY_DIR}/include/
)

set_property(TARGET funchook-withdep PROPERTY
    INTERFACE_LINK_LIBRARIES funchook-distorm funchook-static
)

add_custom_target(copy_luajit_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/deps/dst_luajit
            ${CMAKE_CURRENT_BINARY_DIR}/dst_luajit
)

#-Wl,--section-start=.text=0x10000000
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #set(LINUX_LUAJIT_ENV set "TARGET_SHLDFLAGS=\"-Wl,-Ttext-segment=0x10000000\" CFLAGS=\"-march=native -msse -msse3 -mfpmath=sse -nopie\"" && make -j 8)

    add_custom_command(
        OUTPUT ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/fyp_luajit.so
        #COMMAND make CFLAGS="-march=native" TARGET_SHLDFLAGS="-Wl,-Ttext-segment=0x10000000" -j8
        COMMAND make CFLAGS=-march=native\ -mtune=native\ -mfpmath=both\ -fomit-frame-pointer\ -no-pie -j8
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/dst_luajit/src/libluajit.so
                ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/fyp_luajit.so
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dst_luajit/src
        COMMENT "Building luajit"
    )
    add_custom_target(build_luajit DEPENDS copy_luajit_files ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/fyp_luajit.so)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # set(WINDOWS_LUAJIT_ENV set "TARGET_SHLDFLAGS=\"-Wl,-Ttext-segment=0x10000000\" TARGET_CFLAGS=\"-march=native -msse -msse3 -mfpmath=sse -nopie\""&&make)
    
    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/lib/fyp_luajit.dll
        COMMAND make HOST_CC="gcc" CROSS=x86_64-w64-mingw32- TARGET_SYS=Windows -j8 # TODO: add CMAKE_BUILD_PARALLEL_LEVEL
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/dst_luajit/src/lua51.dll
                ${CMAKE_BINARY_DIR}/lib/fyp_luajit.dll
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dst_luajit/src
        COMMENT "Building luajit"
    )

    add_custom_target(build_luajit DEPENDS copy_luajit_files ${CMAKE_BINARY_DIR}/lib/fyp_luajit.dll)
endif()

add_library(luajit IMPORTED INTERFACE GLOBAL)
add_dependencies(luajit build_luajit)
target_include_directories(luajit INTERFACE ${PROJECT_SOURCE_DIR}/src/deps/dst_luajit/src)
