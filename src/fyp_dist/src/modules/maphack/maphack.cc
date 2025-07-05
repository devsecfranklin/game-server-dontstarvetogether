#include "core.hh"
#include "fmt.hh"

#include "memutil/hook.hh"
#include "sigscan.h"

using namespace fyp;

static PoutFactory log("[fyp:maphack]");
size_t maphack_patch_addr = 0;

SIGNATURE sig_maphack = {
    (char*)"dst_maphack_showArea",
    (char*)"80 b8 f9 01 00 00 00 75 28 31 c0 48 8b 8c 24 98 00 00 00"
};

static int hack_map(lua_State* L) {
    auto& C = Core::get_instance();
    // apply patch removing check
    patch<std::uint8_t>(maphack_patch_addr, 0xeb);
    
    C.lua.luaL_loadstring(L, "TheWorld.minimap.MiniMap:ShowArea(0,0,0, 10000)");
    C.lua.lua_pcall(L, 0, LUA_MULTRET, 0);

    // remove patch removing check
    patch<std::uint8_t>(maphack_patch_addr, 0x75);
    log.red() << "Hacks: maphack" << std::endl;
    return 1;
}

extern "C" FYP_API void mod_init() {
    auto& C = Core::get_instance();
    log.green() << "Installing maphack lua command" << std::endl;

    auto& bounds = C.get_scan_bounds();
    log.green() << "Start: " << std::hex << bounds.start << " end: " << bounds.end << std::endl;
    size_t address = find_signature(bounds.start, bounds.end, sig_maphack);
    if (0 == address) {
        log.red() << "Failed to find patch location" << std::endl;
        return;
    }
    maphack_patch_addr = address + 7;
    log.green() << "address is: " << std::hex << maphack_patch_addr << std::endl;

    C.lua.lua_pushcclosure(C.lua.L, hack_map, 0); // hm = hacks, map
    C.lua.lua_setfield(C.lua.L, -10002, "hm");
}
