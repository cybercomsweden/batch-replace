#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>

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

fs::path open_tmp_file(fs::path path, std::fstream& file) {
	// NOTE: This function is POSIX specific, but can be extended later on to
	//       provide Windows support
	path /= "XXXXXX";

	// We must now convert the path to a C style string pointer. This is not
	// possible using the standard library, and we have to resort to this
	// solution to artificially create one. This assumes path doesn't contain
	// null bytes. By allocating a vector we don't need to manually free the
	// memory later.
	const std::string& path_str = path;
	std::vector<char> dst_path(path_str.begin(), path_str.end());
	dst_path.push_back('\0');


	// Create a temporary file and return its file descriptor and write its
	// path to dst_path. Note that the length is guaranteed to be the same.
	// we don't raise exceptions on error since it's possible to check if the
	// file was opened
	int fd = mkstemp(&dst_path[0]);
	if (fd != -1) {
		path.assign(dst_path.begin(), dst_path.end() - 1);
		file.open(path.c_str(), std::fstream::out);
		close(fd);
	}

	return path;
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
