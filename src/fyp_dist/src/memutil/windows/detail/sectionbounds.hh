#ifndef FYP_MEMUTIL_WINDOWS_DETAIL_SECTIONBOUNDS_HH
#define FYP_MEMUTIL_WINDOWS_DETAIL_SECTIONBOUNDS_HH

#include <psapi.h>
#include "detail/libpe.h"
#include "detail/libpe.cc"

inline std::tuple<size_t, size_t> SectionBounds(std::wstring const& module, std::string const& section_name) {

    auto rv = std::tuple<size_t, size_t>(0,0);

    libpe::IlibpePtr pLibpe { libpe::Createlibpe() };
    
    if(libpe::PEOK != pLibpe->ParsePE(module.c_str()))
        return rv;
    auto sections = pLibpe->GetSecHeaders();

    for (auto s : *sections) {
        std::string name((char*)s.stSecHdr.Name);
        if (name != section_name)
            continue;
        size_t sec_addr = (size_t)GetModuleHandleW(module.c_str()) + s.stSecHdr.VirtualAddress;
        size_t sec_size = s.stSecHdr.Misc.VirtualSize;
        return std::tuple<size_t, size_t>(sec_addr, sec_size);
       
    }
    return std::tuple<size_t, size_t>(0,0);
}

#endif // FYP_MEMUTIL_WINDOWS_DETAIL_SECTIONBOUNDS_HH
