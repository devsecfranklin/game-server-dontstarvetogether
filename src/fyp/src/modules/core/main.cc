#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <format>

#include <inttypes.h>
#include <stdio.h>

#include <cstring>
#include <memory>

#include <funchook.h>
#include <lauxlib.h>
#include <luajit.h>
// #include "config.h"
#include "core.hh"
#include "fmt.hh"
#include "main.hh"
#include "memutil/hook.hh"
#include "memutil/module.hh"
#include "modules/INIReader.h"
#include "signatures.h"
#include "sigscan.h"
#include "updater.hh"

#include "detail/osdep.hh"

using std::format;

namespace fyp {

static PoutFactory plog("[fyp:core:main]");

void load_fyp_modules();

FYP_API void mod_main() {
    auto& C = Core::get_instance();
    C.init();

    ::osdep_init(C.options.console);

    if (C.options.verbose) {
        plog.green() << "Going with these paths:" << std::endl;
        plog.green() << "Root:    " << C.path_root() << std::endl;
        plog.green() << "Bin:     " << C.path_bin() << std::endl;
        plog.green() << "Data:    " << C.path_data() << std::endl;
        plog.green() << "Mods:    " << C.path_mod() << std::endl;
        plog.green() << "UgcMods: " << C.path_mod_ugc() << std::endl;
        plog.green() << "Klei:    " << C.path_klei() << std::endl;
    }
/*
    if (C.options.update) {
        Updater update(C.path_mod_ugc(), C.path_mod());
        for (auto const& p : update.hash_ugcmods())
            plog.green() << p.hash << " " << p.path << std::endl;
    }
    */
    hook_all();
}

void* handle_lua_newstate(void* f, void* ud) {
    auto& C = Core::get_instance();
    // luajit thinks lua_newstate is deprecated and blows up if called
    // luajit also does not support custom allocators, we discard the one we are given.
    // we then call luaL_newstate to create the lua_State object.
    auto L = C.lua.luaL_newstate();

    C.lua.L = L;

    C.lua.lua_pushcclosure(L, jit_on, 0);
    C.lua.lua_setfield(L, -10002, "jit_on");
    C.lua.lua_pushcclosure(L, jit_off, 0);
    C.lua.lua_setfield(L, -10002, "jit_off");

    plog.green() << "Jit turned on, enter jit_on() or jit_off() to set" << std::endl;
    C.lua.luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);

    plog.cyan() << "lua_State created, loading fyp_modules" << std::endl;
    load_fyp_modules();

    return (void*)L;
}

int debug_shim(lua_State* L) { return 1; }

void* handle_dst_lua_debug_getsize(lua_State* L) {
    auto& C = Core::get_instance();
    C.lua.lua_getfield(L, 0xffffd8ee, "debug");
    C.lua.lua_createtable(L, 0, 0);
    C.lua.lua_pushlightuserdata(L, 0);
    C.lua.lua_pushcclosure(L, debug_shim, 1);
    C.lua.lua_setfield(L, 1, "getsize");
    C.lua.lua_settop(L, 0xfffffffe);
    return 0;
}

void* load_luajit() {
    auto libpath = fs::current_path() / PATH_LUAJIT;
    return ModuleOpen(libpath);
}

bool find_export(void* handle, std::string const& function_name, void** dst) {
    *dst = ModuleFindExport(handle, function_name);
    return (*dst != nullptr);
}

void hook_all() {
    auto& C = Core::get_instance();
    auto h_luajit = load_luajit();

    auto SCAN_START = C.get_scan_bounds().start;
    auto SCAN_END = C.get_scan_bounds().end;

    if (C.options.verbose) {
        plog.green() << "CWD: " << fs::current_path() << std::endl;
    }
    plog.green() << "Loading luajit..." << std::endl;

    // clang-format off
    std::vector<std::pair<std::string, void**>> luajit_exports = {
        { "luaL_newstate",         (void**)&C.lua.luaL_newstate },
        { "lua_newstate",          (void**)&C.lua.lua_newstate },
        { "luaJIT_setmode",        (void**)&C.lua.luaJIT_setmode },
        { "lua_pushcclosure",      (void**)&C.lua.lua_pushcclosure },
        { "lua_setfield",          (void**)&C.lua.lua_setfield },
        { "lua_getfield",          (void**)&C.lua.lua_getfield },
        { "lua_createtable",       (void**)&C.lua.lua_createtable },
        { "lua_pushlightuserdata", (void**)&C.lua.lua_pushlightuserdata },
        { "lua_settop",            (void**)&C.lua.lua_settop },
        { "lua_pooshboolean",      (void**)&C.lua.lua_pushboolean },
        { "luaL_loadstring",       (void**)&C.lua.luaL_loadstring },
        { "lua_pcall",             (void**)&C.lua.lua_pcall }
    };
    // clang-format on

    for (auto& i : luajit_exports)
        find_export(h_luajit, i.first, i.second);

    if (C.options.verbose) {
        plog.cyan() << format("luajit loaded at: 0x{:x}", (size_t)h_luajit) << std::endl;
        plog.cyan() << format("luajit luaL_newstate at: 0x{:x}", (size_t)C.lua.luaL_newstate)
                   << std::endl;
        plog.cyan() << format("luajit lua_newstate at: 0x{:x}", (size_t)C.lua.lua_newstate)
                   << std::endl;
        plog.cyan() << format("luajit luaJIT_setmode at: 0x{:x}", (size_t)C.lua.luaJIT_setmode)
                   << std::endl;
    }

    ScanBounds scan_bounds = { SCAN_START, SCAN_END };
    C.set_scan_bounds(scan_bounds);

    std::vector<SIGNATURE> not_found;
    int numsigs = sizeof(SIGS_ALL) / sizeof(SIGNATURE);

    for (auto& sig : SIGS_ALL) {

        std::string fname(sig.function_name);

        size_t address = find_signature(SCAN_START, SCAN_END, sig);

        if (address == 0) {
            not_found.push_back(sig);
            continue; // he'll walk it off
        }

        if (fname == "lua_newstate") {
            if (C.options.verbose)
                plog.cyan() << format("{:<22}[0x{:016x}] -> 0x{:016x} [plogic modified]"
                        , sig.function_name
                        , address
                        , (size_t)handle_lua_newstate)
                           << std::endl;
            ghook(address, (size_t)handle_lua_newstate);
            continue;
        }

        if (fname == "dst_lua_debug_getsize") {
            if (C.options.verbose)
                plog.green() << "Installing debug:getsize stub" << std::endl;
            ghook(address, (size_t)handle_dst_lua_debug_getsize);
            continue;
        }

        if (fname == "luaopen_debug") {
            // This one is tricky, the function is a very small accessor.
            // The signaure matches more than one function. So we find the second sig.
            // The first one is luaopen_os
            // The second one is luaopen_debug
            // The third one is luaopen_table
            address = find_signature(address + 8, SCAN_END, sig);
        }

        size_t jitaddress;
        find_export(h_luajit, sig.function_name, (void**)&jitaddress);
        if (jitaddress == 0) {

            plog.red() << format("LuaJIT export not found: {:<22}[0x{:016x}] -> 0x{:016x}", sig.function_name, address, jitaddress)
                      << std::endl;
        }

        if (C.options.verbose)
            plog.cyan() << format("{:<22}[0x{:016x}] -> 0x{:016x}", sig.function_name, address, jitaddress)
                       << std::endl;
        ghook(address, jitaddress);
    }

    if (not_found.size() == 0)
        plog.green() << "Found all signatures" << std::endl;
    else {
        plog.red() << format("Missed {} signatures out of {}", not_found.size(), numsigs)
                  << std::endl;
        for (auto& sig : not_found)
            plog.cyan() << "Missed: " << sig.function_name << std::endl;
    }
    funchook_install(C.hook_ctx, 0);
}

static int jit_on(lua_State* L) {
    Core::get_instance().lua.luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_ON);
    plog.red() << "Jit: ON" << std::flush
              << std::endl; // TODO: not sure why, but color persists till end of next
                            // line
    return 1;
}

static int jit_off(lua_State* L) {
    Core::get_instance().lua.luaJIT_setmode(L, 0, LUAJIT_MODE_ENGINE | LUAJIT_MODE_OFF); // TODO: same
    plog.red() << "Jit: OFF" << std::flush << std::endl;
    return 1;
}

} // namespace fyp
