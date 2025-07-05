#ifndef FYP_MEMUTIL_WINDOWS_DETAIL_PAGEPROTECT_HH
#define FYP_MEMUTIL_WINDOWS_DETAIL_PAGEPROTECT_HH

extern "C" {
    #include <windows.h>
}
//#include <Memoryapi.h>
#define PROT_READ 0
#define PROT_WRITE PAGE_READWRITE
#define PROT_EXEC PAGE_EXECUTE_READWRITE

inline int PageProtect(size_t addr, size_t size, int flags) {
    uint32_t old_protect = 0;
    auto rv = VirtualProtect((LPVOID)addr, size, PAGE_EXECUTE_READWRITE, (PDWORD)&old_protect);
    return (int) rv;
}

#endif // FYP_MEMUTIL_WINDOWS_DETAIL_PAGEPROTECT_HH
