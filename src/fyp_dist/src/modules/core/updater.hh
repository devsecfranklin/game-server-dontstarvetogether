#ifndef FYP_MODULES_MAIN_UPDATER_HH
#define FYP_MODULES_MAIN_UPDATER_HH

#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace fyp {

struct FileHash {
    fs::path path;
    std::string hash;
};

struct Updater {
    Updater(const Updater&) = delete;
    Updater() = delete;
    Updater(fs::path const& path_mod_ugc, fs::path const& path_mod);
    ~Updater();

    fs::path _path_mod_ugc;
    fs::path _path_mod;

    std::vector<fs::path> index_mod();
    std::vector<fs::path> index_mod_ugc();
 
    std::vector<FileHash> hash_ugcmods();

    std::string GetRemoteVersion();
    std::string GetLocalVersion();

    bool start_torrent(std::string const& magnet);
};


}

#endif // FYP_MODULES_MAIN_UPDATER_HH
