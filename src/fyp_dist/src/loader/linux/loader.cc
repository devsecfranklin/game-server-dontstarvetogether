#include <filesystem>
#include <iostream>
#include <stdint.h>
#include <dlfcn.h>

#include <funchook.h>

#include "signatures.h"
#include "sigscan.h"
#include "memutil/module.hh"

const char* FYP_MAIN_LIB = "fyp_main.so";

int handle_dst_server_main(int argc, char** argv, char** envp);
int handle_dst_client_main(int argc, char** argv);
decltype(&handle_dst_server_main) orig_dst_server_main;
decltype(&handle_dst_client_main) orig_dst_client_main;

typedef void (*t_mod_main)();

bool loaded = false;

void load_fyp() {
    std::cout << "[fyp:loader] Loading main module" << std::endl;
    auto cwd = ProcessPath().parent_path();
    std::string path_bolt = cwd / FYP_MAIN_LIB;
    std::cout << "[fyp:loader] Loading from: " << path_bolt << std::endl;

    void* handle_bolt = dlopen(path_bolt.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (NULL == handle_bolt) {
        std::cout << "[fyp:loader] Error loading main module"
                  << std::endl
                  << dlerror() << std::endl;
    }

    t_mod_main entry = (t_mod_main)dlsym(handle_bolt, "mod_main");

    if (entry != 0)
        entry();
    else
        std::cout << "[fyp:loader] Error getting pointer to main" << std::endl;
}


int handle_dst_server_main(int argc, char** argv, char** envp) {
    if (true == loaded)
        return orig_dst_server_main(argc, argv, envp);
    
    loaded = true;
    load_fyp();

    return orig_dst_server_main(argc, argv, envp);
}

int handle_dst_client_main(int argc, char** argv) {
    if (true == loaded)
        return orig_dst_client_main(argc, argv);
    
    loaded = true;
    load_fyp();
    
    return orig_dst_client_main(argc, argv);
}

struct Loader {

    Loader() {
        auto path = ProcessPath();
        if (path.filename().string().rfind("dontstarve_") != 0) {
            std::cout << "[fyp:loader] Loader inside of: " << path << std::endl;
            std::cout << "[fyp:loader] Probably not dst client or server, ignoring" << std::endl;
            return;
        }
        else {
            std::cout << "[fyp:loader] Process path: " << path << std::endl;
        }

        std::cout << "==============\n" << path.filename().string() << std::endl;
        auto bounds = SectionBounds(path.wstring(), ".text"); // TODO: sectionbounds assumes .text atm
        auto scan_start = std::get<0>(bounds); // TODO: get<0> is a bit weird, make it simple
        auto scan_end   = std::get<1>(bounds) + scan_start;

        _hook_ctx = funchook_create();
        for (auto& sig : SIGS_ALL) {
            if (std::string(sig.function_name) == "main_server") {
                orig_dst_server_main = (decltype(&handle_dst_server_main))
                    find_signature(scan_start, scan_end, sig);
                funchook_prepare(_hook_ctx,
                    (void**)&orig_dst_server_main,
                    (void*)handle_dst_server_main);
            }
            if (std::string(sig.function_name) == "main_client") {
                orig_dst_client_main = (decltype(&handle_dst_client_main))
                    find_signature(scan_start, scan_end, sig);
                funchook_prepare(_hook_ctx,
                    (void**)&orig_dst_client_main,
                    (void*)handle_dst_client_main);
            }
        }

        funchook_install(_hook_ctx, 0);
    }

    // Called on process exit
    ~Loader() {
        funchook_destroy(_hook_ctx);
    }

private:
    funchook_t* _hook_ctx;
};

Loader l;
