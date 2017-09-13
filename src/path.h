#ifndef PATH_H
#define PATH_H

#include <experimental/filesystem>
#include <set>
#include <string>
#include <vector>

namespace fs = std::experimental::filesystem;

namespace Path {

bool has_extension(fs::path const &, std::string const &);
bool has_valid_extension(fs::path const &, std::vector<std::string> const &);
fs::path open_tmp_file(fs::path path, std::fstream& file);
std::set<fs::path> list_files(std::vector<fs::path> const &, std::vector<std::string> const &);

}

#endif
