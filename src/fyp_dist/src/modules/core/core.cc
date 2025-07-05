#include <iostream>

#include "fmt.hh"
#include "core.hh"
#include "memutil/module.hh"

namespace fyp {

static PoutFactory log("[fyp:core:core]");

Core::Core() {

    hook_ctx = funchook_create();
}

// TODO: Why is this exported??
Core::~Core() {
    log.green() << "Cleanup time..." << std::endl;
    //funchook_destroy(hook_ctx);
    exit(1); // OK so apparently the game doesn't actually close the process, so we do it.
    // TODO: yeah so that doesn't appear to kill the process @_@
    log.red() << "ITS JUST A FLESH WOUND, HAVE AT YOU" << std::endl;
}

namespace fs = std::filesystem;
fs::path Core::path_root() {
    return _path_root;
}

fs::path Core::path_bin() {
    return _path_bin;
}

fs::path Core::path_data() {
    return _path_data;
}

fs::path Core::path_mod() {
    return _path_mod;
}

fs::path Core::path_mod_ugc() {
    return _path_mod_ugc;
}

fs::path Core::path_klei() {
    return _path_klei;
}


void Core::set_scan_bounds(ScanBounds const& bounds) {
    _scan_bounds = bounds;
}

const ScanBounds& Core::get_scan_bounds() {
    return _scan_bounds;
}

void Core::init() {
    namespace fs = std::filesystem;
    //    log.green() << "VERSION: " << format("%i.%i")
    //        % FYP_VERSION_MAJOR
    //        % FYP_VERSION_MINOR
    //        << std::endl;
    _path_root = fs::current_path().parent_path();
    _path_bin = _path_root / "bin64";
    _path_data = _path_root / "data";
    _path_mod = _path_root / "mods";
    _path_mod_ugc = _path_root.parent_path().parent_path()
        / "workshop" / "content" / "322330";
    _path_klei = ""; // TODO: in a platform agnostic way, get the "home" folder
    // INIReader reader(
    auto ini_path = _path_bin / fs::path("fyp_config.ini");
    INIReader reader(ini_path.string());
    int perr = reader.ParseError();

    options.console    = reader.GetBoolean("main", "console", true);
    options.update     = reader.GetBoolean("main", "update", true);
    options.update_url = reader.Get("main", "update_url", "http://localhost:8000/test");
    options.verbose    = reader.GetBoolean("main", "verbose", true);

    if (perr)
        log.red() << "Ini parser failed, does the file exist? : " << perr
                  << std::endl;

    auto path = ProcessPath();
    auto bounds = SectionBounds(path.wstring(), ".text");
    ScanBounds sb = { std::get<0>(bounds), std::get<0>(bounds) + std::get<1>(bounds) };
    _scan_bounds = sb;

}

} // namespace fyp
