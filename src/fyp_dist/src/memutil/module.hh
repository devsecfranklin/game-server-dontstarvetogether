#ifndef FYP_MEMUTIL_MODULE_HH
#define FYP_MEMUTIL_MODULE_HH


#include <tuple>
#include <string>
#include <filesystem>

#include "detail/openmodule.hh"
#include "detail/sectionbounds.hh"
#include "detail/processpath.hh"

// Loads the module into memory if neccessary
// Returns a handle to the module
inline void* ModuleOpen(std::string const& path);
inline void* ModuleFindExport(void* mod, std::string const& export_name);

inline std::filesystem::path ProcessPath();
inline std::tuple<size_t, size_t> SectionBounds(std::wstring const& module, std::string const& section_name);

#endif /* FYP_MEMUTIL_MODULE_HH */
