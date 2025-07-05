#include <sys/mman.h>
#include <unistd.h>

inline int PageProtect(size_t addr, size_t size, int flags) {
    auto pagesize = sysconf(_SC_PAGESIZE);
    size_t start = addr & ((0UL - 1UL) ^ (pagesize - 1));
    return mprotect((void*)start, pagesize, flags);
}
