#ifndef FYP_MEMUTIL_WINDOWS_DETAIL_OPENMODULE_HH
#define FYP_MEMUTIL_WINDOWS_DETAIL_OPENMODULE_HH

extern "C" {
#include <windows.h>
}

inline void* ModuleOpen(std::string const& path) {
    return LoadLibraryA(path.c_str()); // TODO: dig into flags
}

inline void* ModuleOpen(std::wstring const& path) {
    return LoadLibraryW(path.c_str()); // TODO: dig into flags
}

inline void* ModuleFindExport(void* mod, std::string const& export_name) {
    return (void*)GetProcAddress((HMODULE)mod, export_name.c_str());
}

#endif // FYP_MEMUTIL_WINDOWS_DETAIL_OPENMODULE_HH
