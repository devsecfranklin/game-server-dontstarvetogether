#include <dlfcn.h>
#include <string>

inline void* ModuleOpen(std::string const& path) {
    return dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL); // TODO: dig into flags
}

inline void* ModuleFindExport(void* mod, std::string const& export_name) {
    return dlsym(mod, export_name.c_str());
}
