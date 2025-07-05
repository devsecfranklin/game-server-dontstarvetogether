#pragma once

#if defined(_WIN64)
    #ifdef BUILD_FYP_LIB
        #ifndef FYP_EXPORT
            #define FYP_EXPORT __declspec(dllexport)
        #endif
        #ifndef FYP_IMPORT
            #define FYP_IMPORT
        #endif
    #else
        #ifndef FYP_EXPORT
            #define FYP_EXPORT __declspec(dllimport)
        #endif
        #ifndef FYP_IMPORT
            #define FYP_IMPORT FYP_EXPORT
        #endif
    #endif
#else
    #ifndef FYP_EXPORT
        #define FYP_EXPORT __attribute__ ((visibility("default")))
    #endif
    #ifndef FYP_IMPORT
        #define FYP_IMPORT FYP_EXPORT
    #endif
#endif

#ifndef FYP_API
#if defined(_WIN64)
#define FYP_API __declspec(dllexport)
#else
#define FYP_API __attribute__((__visibility__("default")))
#endif
#endif


#include <filesystem>
#include <functional>

#include <funchook.h>

#include "lauxlib.h"
#include "lua.h"
#include "luajit.h"
#include "lualib.h"

#include "modules/INIReader.h"

typedef void (*ModuleInitPtr)();

namespace fyp {

typedef std::function<bool()> on_lua_callback;
typedef std::function<bool()> on_main_callback;

namespace fs = std::filesystem;

struct FYP_EXPORT CoreOptions {
    bool console;
    bool update;
    bool verbose;
    std::string update_url;
};

struct FYP_EXPORT ScanBounds {
    size_t start;
    size_t end;
};

struct FYP_EXPORT DstPaths {
    fs::path root;
    fs::path bin;
    fs::path data;
    fs::path mods;
    fs::path ugcmods;
};

struct FYP_EXPORT LuaPointers {
    lua_State* L;
    decltype(&::luaL_newstate) luaL_newstate;
    decltype(&::lua_newstate) lua_newstate;
    decltype(&::luaJIT_setmode) luaJIT_setmode;
    decltype(&::lua_pushcclosure) lua_pushcclosure;
    decltype(&::lua_getfield) lua_getfield;
    decltype(&::lua_setfield) lua_setfield;
    decltype(&::lua_createtable) lua_createtable;
    decltype(&::lua_pushlightuserdata) lua_pushlightuserdata;
    decltype(&::lua_settop) lua_settop;
    decltype(&::lua_pushboolean) lua_pushboolean;
    decltype(&::luaL_loadstring) luaL_loadstring;
    decltype(&::lua_pcall) lua_pcall;
};

class FYP_EXPORT Core {
public:
    void init();
    fs::path path_root();
    fs::path path_bin();
    fs::path path_data();
    fs::path path_mod();
    fs::path path_mod_ugc();
    fs::path path_klei();

    static Core& get_instance() {
        static Core core;
        return core;
    }

    const ScanBounds& get_scan_bounds();
    void set_scan_bounds(ScanBounds const&);
    const DstPaths& get_paths();

    LuaPointers lua;
    CoreOptions options;
    INIReader conf;
    fs::path dst_root;
    funchook_t* hook_ctx;
    //    std::ios_base::Init _ios_base_initializer;
private:
    Core();
    ~Core();

    ScanBounds _scan_bounds;
    DstPaths _dst_paths;

    fs::path _path_root;
    fs::path _path_data;
    fs::path _path_bin;
    fs::path _path_mod;
    fs::path _path_mod_ugc;
    fs::path _path_klei;

    std::vector<std::function<bool()>> _on_lua_cbs;
    std::vector<std::function<bool()>> _on_main_cbs;
    Core(Core const&) = delete;
    Core& operator=(Core const&) = delete;
};

} // namespace fyp
