#ifndef FYP_MEMUTIL_LINUX_DETAIL_SECTIONBOUNDS_HH
#define FYP_MEMUTIL_LINUX_DETAIL_SECTIONBOUNDS_HH

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <optional>
#include <vector>
#include <stdexcept>
#include <link.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <elf.h>

#include "processpath.hh"

// TODO: figure out why this is getting linked more than once
inline std::tuple<size_t, size_t> SectionBounds(std::wstring const& module_wide, std::string const& section_name) {
    namespace fs = std::filesystem;

    std::string module_name;
    // default is to return section bounds for main exe
    std::filesystem::path module_path(module_wide);
    module_name = module_path.filename().string();

    // we need to track some context for lambda callback
    struct MatchContext {
        std::string target_name;
        std::string section_name;
        std::optional<std::tuple<size_t, size_t>> result;
    } context{module_name, section_name, std::nullopt};

    dl_iterate_phdr([](dl_phdr_info* info, size_t, void* data) -> int {
        auto* ctx = static_cast<MatchContext*>(data);

        std::string path = info->dlpi_name;
        if (path.empty()) path = ProcessPath();
   
        std::string base = std::filesystem::path(path).filename().string();
        if (ctx->target_name != base) {
            return 0;
        }

        int fd = open(path.c_str(), O_RDONLY);
        if (fd < 0)
            return 0;

        struct stat st;
        if (fstat(fd, &st) < 0) {
            close(fd);
            return 0;
        }

        void* map = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);
        if (map == MAP_FAILED)
            return 0;

        auto* ehdr = reinterpret_cast<Elf64_Ehdr*>(map);
        auto* shdrs = reinterpret_cast<Elf64_Shdr*>((char*)map + ehdr->e_shoff);
        const char* shstrtab = (char*)map + shdrs[ehdr->e_shstrndx].sh_offset;

        for (int i = 0; i < ehdr->e_shnum; ++i) {
            const char* name = shstrtab + shdrs[i].sh_name;
            if (ctx->section_name == name) {
                size_t start = info->dlpi_addr + shdrs[i].sh_addr;
                size_t end = shdrs[i].sh_size;
                ctx->result = std::make_tuple(start, end);
                munmap(map, st.st_size);
                return 1;  // found so stop
            }
        }

        munmap(map, st.st_size);
        return 0;
    }, &context);

    if (!context.result.has_value()) {
        throw std::runtime_error("Module or section not found: " + module_name + " : " + section_name);
    }
    return context.result.value();
}

#endif // FYP_MEMUTIL_LINUX_DETAIL_SECTIONBOUNDS_HH
