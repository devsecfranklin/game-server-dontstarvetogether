#ifndef FYP_MODULES_MAIN_DETAIL_LINUX_OSDEP_HH
#define FYP_MODULES_MAIN_DETAIL_LINUX_OSDEP_HH

#include <cstddef>

static const char* PATH_LUAJIT = "fyp_luajit.so";

const size_t BASE_ADDR = 0x0040b770;
const size_t SCAN_DIST = 0x00484062;

inline void osdep_init(bool console) {
    return;
}

#endif /* FYP_MODULES_MAIN_DETAIL_LINUX_OSDEP_HH */
