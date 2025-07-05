#ifndef FYP_MEMUTIL_LINUX_DETAIL_PROCESSPATH_HH
#define FYP_MEMUTIL_LINUX_DETAIL_PROCESSPATH_HH

#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>

#include <filesystem>

const int MAX_PATH_LEN = 256;

// TODO: Why inline again?
inline std::filesystem::path ProcessPath() {
    char cpath[MAX_PATH_LEN] = {0};
    auto length = readlink("/proc/self/exe", cpath, sizeof(cpath));
    if (length < 0)
        return "";

    if (length >= sizeof(cpath))
        return "";

    return std::filesystem::path(cpath);
}

#endif // FYP_MEMUTIL_LINUX_DETAIL_PROCESSPATH_HH
