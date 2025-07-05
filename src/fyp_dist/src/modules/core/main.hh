#ifndef FYP_MODULES_MAIN_MAIN_HH
#define FYP_MODULES_MAIN_MAIN_HH

#include <lua.h>
#ifndef FYP_API
#if defined(_WIN64)
#define FYP_API __declspec(dllexport)
#else
#define FYP_API __attribute__((__visibility__("default")))
#endif
#endif

#include <filesystem>
#include <functional>

namespace fyp {

extern "C" FYP_API void mod_main();
void hook_all();
void* load_luajit();
bool find_luajit_export(const char* function_name, void** dst);
static int jit_on(lua_State* L);
static int jit_off(lua_State* L);

} // namespace fyp

#endif // FYP_MODULES_MAIN_MAIN_HH
