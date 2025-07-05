#include <filesystem>
#include <fstream>
#include <iterator>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "boost/asio/strand.hpp"
#include "core.hh"
#include "fmt.hh"
#include "picosha2.h"
#include "updater.hh"

namespace fyp {

namespace fs = std::filesystem;

Updater::Updater(fs::path const& path_mod_ugc, fs::path const& path_mod) {
    _path_mod_ugc = path_mod_ugc;
    _path_mod = path_mod;
}

Updater::~Updater() {
}

std::vector<fs::path> Updater::index_mod() {
    std::vector<fs::path> mod_paths;
    for (auto const& m : fs::directory_iterator(_path_mod)) {
        if (m.path().filename().string().starts_with("workshop-"))
            mod_paths.push_back(m.path());
    }

    return mod_paths;
}

std::vector<fs::path> Updater::index_mod_ugc() {
    std::vector<fs::path> mod_paths;
    for (auto const& m : fs::directory_iterator(_path_mod_ugc)) {
        mod_paths.push_back(m.path());
    }

    return mod_paths;
}

FileHash hash_one(fs::path const& one) {
    FileHash file_hash;
    file_hash.path = one;

    std::ifstream cur_file(one, std::ios::binary);
    std::vector<unsigned char> hash(picosha2::k_digest_size);
    picosha2::hash256(cur_file, hash.begin(), hash.end());

    file_hash.hash = picosha2::bytes_to_hex_string(hash.begin(), hash.end());
    return file_hash;
}

std::vector<FileHash> Updater::hash_ugcmods() {

    boost::asio::thread_pool tp;

    auto paths = index_mod();
    auto paths_ugc = index_mod_ugc();
    paths.insert(
            paths.end(),
            std::make_move_iterator(paths_ugc.begin()),
            std::make_move_iterator(paths_ugc.end()));

    std::vector<std::future<FileHash>> future_file_hashes;

    for (auto const& module : paths) {
        for (auto const& mp : fs::recursive_directory_iterator(module)) {
            if (false == mp.is_regular_file())
                continue;

            auto f = boost::asio::post(tp, std::packaged_task<FileHash()>(std::bind(hash_one, mp.path())));
            future_file_hashes.emplace_back(std::move(f));
        }
    }

    // Collected futures into vector to return. This step is probably avoidable.
    std::vector<FileHash> file_hashes;
    for (auto& f : future_file_hashes) {
        file_hashes.emplace_back(std::move(f.get()));
    }

    return file_hashes;
}

}
