#ifndef FYP_MEMUTIL_HOOK_HH
#define FYP_MEMUTIL_HOOK_HH

#include <cstring>
#include <iostream>
#include <stdint.h>
#include <string>
#include <unistd.h>

#include "detail/pageprotect.hh"

// JMP to 64bit displacement while avoiding registers
// pushes one 32bit half to stack then movs other 32bit half to stack+4
// returns to pushed value, popping the value off the stack
const char RETJMP_TEMPLATE[] = "\x68\xff\xff\xff\xff" // pushq (address high)
                               "\xc7\x44\x24\x04\xff\xff\xff\xff" // movl [rsp+4], (address low)
                               "\xc3"; // retq

const char RELJMP_TEMPLATE[] = "\xe9" // jmp rel32
                               "\xff\xff\xff\xff"; // offset rel to (RIP+len(RELJMP_TEMPLATE))

const char RAXJMP_TEMPLATE[] = "\x48\xb8\xff\xff\xff\xff\xff\xff\xff\xff" // mov rax, 64 bit val
                               "\xff\xe0"; // jmp rax

// TODO: Do not leave memory read/write/exec to be safer
int PageProtect(size_t addr, size_t size, int flags);
void ghook(size_t from, size_t to);

template <
    typename T,
    typename = std::enable_if_t<
        std::is_trivially_copyable<T>::value>>
int patch(size_t address, T data) {
    auto size = sizeof(T);
    auto rv = PageProtect(address, size, PROT_READ | PROT_WRITE | PROT_EXEC);
    memcpy((void*)address, &data, size);
    return 0;
}

template <
    typename T,
    typename = std::string>
int patch(size_t address, std::string const& data) {
    auto size = data.size();
    auto rv = PageProtect(address, size, PROT_READ | PROT_WRITE | PROT_EXEC);
    memcpy((void*)address, data.c_str(), size);
    return 0;
}

inline void hook_abs(size_t from, size_t to) {
    uint8_t* ptr = (uint8_t*)from;
    auto rv = PageProtect(
        from,
        sizeof(RETJMP_TEMPLATE),
        PROT_READ | PROT_WRITE | PROT_EXEC);

    memcpy(ptr, RETJMP_TEMPLATE, sizeof(RETJMP_TEMPLATE));
    uint32_t to_high = to >> 32;
    uint32_t to_low = to & 0xffffffff;
    memcpy(ptr + 1, &to_low, 4);
    memcpy(ptr + 9, &to_high, 4);

    rv = PageProtect(
        from,
        sizeof(RETJMP_TEMPLATE),
        PROT_READ | PROT_EXEC);
}

inline void hook_rel(size_t from, size_t to) {
    uint8_t* ptr = (uint8_t*)from;
    auto rv = PageProtect(
        from,
        sizeof(RELJMP_TEMPLATE),
        PROT_READ | PROT_WRITE | PROT_EXEC);

    uint32_t delta = to - from - sizeof(RELJMP_TEMPLATE);
    memcpy(ptr, RELJMP_TEMPLATE, sizeof(RELJMP_TEMPLATE));
    memcpy(ptr + 1, &delta, 4);

    rv = PageProtect(
        from,
        sizeof(RELJMP_TEMPLATE),
        PROT_READ | PROT_EXEC);
}

inline void hook_rax(size_t from, size_t to) {
    uint8_t* ptr = (uint8_t*)from;
    auto rv = PageProtect(
        from,
        sizeof(RAXJMP_TEMPLATE),
        PROT_READ | PROT_WRITE | PROT_EXEC);

    memcpy(ptr, RAXJMP_TEMPLATE, sizeof(RAXJMP_TEMPLATE));
    memcpy(ptr + 2, &to, 8);

    rv = PageProtect(
        from,
        sizeof(RELJMP_TEMPLATE),
        PROT_READ | PROT_EXEC);

}

inline int calc_rel(size_t from, size_t to) {
    //    printf("from: %016llx\nto: %016llx\n", from, to);

    int negative = from > to ? 1 : 0;
    size_t delta;
    if (negative)
        delta = from - to;
    else
        delta = to - from;

    if (delta & 0xffffffff00000000)
        return 0; // does not fit into 32 bits

    if (negative)
        return ((int)delta) * -1;

    return (int)delta;
}

// not thread safe
inline void ghook(size_t from, size_t to) {
    // check if from-to fits into a 32-bit value
    int rel = calc_rel(to, from + sizeof(RELJMP_TEMPLATE));

    hook_rax(from, to);
    return; // lets just try this for now

    if (rel) {
        std::cout << "optimizing hook..." << std::endl;
        hook_rel(from, to);
    } else {
        //	std::cout << "using slow hook..." << std::endl;
        hook_abs(from, to);
    }
}

#endif // FYP_MEMUTIL_HOOK_HH
