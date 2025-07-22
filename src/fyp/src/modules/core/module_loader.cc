#include <filesystem>
#include <string>
#include <vector>

#include "core.hh"
#include "fmt.hh"
#include "main.hh"
#include "memutil/hook.hh"
#include "memutil/module.hh"

namespace fyp {
static PoutFactory plog("[fyp:main]");
std::vector<std::string> ignored_modules = {
    "fyp_main",
    "fyp_luajit",
    "fyp_loader",
    "fyp_config"
};

void load_fyp_modules() {
    Core& C = Core::get_instance();
    auto path = C.path_bin();
    std::filesystem::directory_iterator dir { path };
    std::vector<std::filesystem::path> candidates;

    for (auto const& file : dir) {
        auto strname = file.path().filename().string();
        if (strname.find("fyp_") == std::string::npos)
            continue;

        bool valid = true;
        // TODO: use a vector of blocked modules
        for (auto const& i : ignored_modules) {
            if (strname.find(i) != std::string::npos) {
                valid = false;
                break;
            }
        }
        if (true == valid)
            candidates.push_back(file);
    }

    for (auto const& file : candidates) {
        plog.cyan() << "loading: " << file << std::endl;

        auto module = ModuleOpen(file.string());
        if (0 == module)
            plog.red() << "failed to load: " << file.filename() << std::endl;

        auto init_func = (ModuleInitPtr)ModuleFindExport(module, "mod_init");
        if (0 == init_func)
            plog.red() << "failed to find init func" << std::endl;
        else
            init_func();
    }
}

} // namespace Fyp
