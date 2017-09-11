#ifndef REPLACE_H
#define REPLACE_H

#include <experimental/filesystem>
#include <string>
#include <utility>
#include <vector>

namespace fs = std::experimental::filesystem;

std::vector<std::pair<std::string, std::string>> replacements_from_file(fs::path const &);

#endif
