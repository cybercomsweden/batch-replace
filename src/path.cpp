#include <experimental/filesystem>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "path.h"

namespace fs = std::experimental::filesystem;

namespace Path {

bool has_extension(fs::path const &path, std::string const &extension) {
	return path.extension().string() == extension;
}

bool has_valid_extension(
	fs::path const &path,
	std::vector<std::string> const &extensions
) {
	// All extensions are valid if there are no valid extensions specified
	if (!extensions.size()) {
		return true;
	}

	for (auto& ext : extensions) {
		if (has_extension(path, ext)) {
			return true;
		}
	}
	return false;
}

std::set<fs::path> list_files(
	std::vector<fs::path> const &paths,
	std::vector<std::string> const &extensions
) {
	std::set<fs::path> filenames;
	for (auto& path : paths) {
		for (auto& p: fs::recursive_directory_iterator(path)) {
			fs::path current_path = p.path();
			if (has_valid_extension(current_path, extensions)) {
				filenames.insert(current_path);
			}
		}
	}
	return filenames;
}

}
