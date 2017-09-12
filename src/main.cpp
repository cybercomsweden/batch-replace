#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "path.h"
#include "text_replacement.h"

namespace fs = std::experimental::filesystem;

void print_help(std::string const &exec_name) {
	std::cerr << exec_name << " [options] <replacement-file> [files..]" << std::endl;
	std::cerr << std::endl;

	std::cerr << "Batch replace allows you to perform advanced search and replace on arbitrary" << std::endl;
	std::cerr << "text files." << std::endl;
	std::cerr << std::endl;

	std::cerr << "Arguments:" << std::endl;
	std::cerr << "    <replacement-file>  A text file containing things to replace. If the" << std::endl;
	std::cerr << "                        file name starts with a hyphen (-) use -- as the." << std::endl;
	std::cerr << "                        last option." << std::endl;
	std::cerr << "    [files..]           Optional list of files and directories to perform" << std::endl;
	std::cerr << "                        replacements in. File extentions can be specified" << std::endl;
	std::cerr << "                        using --ext." << std::endl;
	std::cerr << std::endl;

	std::cerr << "Options:" << std::endl;
	std::cerr << "    -e, --ext      Extensions to search for. For multiple extentions, " << std::endl;
	std::cerr << "                   use -e or --ext for each one." << std::endl;
	std::cerr << "    -h, --help     Display this help message." << std::endl;
	std::cerr << "    -v, --verbose  Display information on what the program is doing." << std::endl;
}


int main(int argc, char* argv[]) {
	// Parse all options
	bool help = false;
	bool verbose = false;
	std::vector<std::string> extensions;
	int current_idx = 1;
	for (; current_idx < argc; current_idx++) {
		std::string option(argv[current_idx]);

		// Check if we're still parsing options
		if (option[0] != '-') {
			break;
		} else if (option == "--") {
			current_idx++;
			break;
		}

		// We're still reading options
		if (option == "-h" || option == "--help") {
			fs::path self_path(argv[0]);
			print_help(self_path.filename().string());
			return 0;
		} else if (option == "-e" || option == "--ext") {
			// We need to consume the next arg, and verify that we don't read
			// out of range
			current_idx++;
			if (current_idx >= argc) {
				std::cerr << "No extensions provided to --ext" << std::endl;
				return 3;
			}
			std::string exts = ".";
			exts += std::string(argv[current_idx]);
			std::cout << "Got ext: " << exts << std::endl;
			extensions.push_back(exts);
		} else if (option == "-v" || option == "--verbose") {
			verbose = true;
		} else {
			std::cerr << "Got unknown option '" << option << "'" << std::endl;
			return 2;
		}
	}

	// If we have consumed all arguments now there is no replacements file
	if (current_idx >= argc) {
		std::cerr << "No replacements file specified" << std::endl;
		return 1;
	}

	// Read replacements from the given file and return them in a properly
	// sorted
	fs::path replacements_path(argv[current_idx++]);
	std::vector<std::pair<std::string, std::string>>
		replacements = replacements_from_file(replacements_path);

	if (verbose) {
		std::cout << "Loaded replacements from " << replacements_path << std::endl;
		for (auto& r : replacements) {
			std::cout << "Will replace \"" << r.first << "\" with \"" << r.second << "\"" << std::endl;
		}
	}

	// Check if we managed to load any replacements from the given file
	if (!replacements.size()) {
		std::cerr << "No replacements found in " << replacements_path << std::endl;
		return 4;
	}


	// Check if there where any files or folders specified
	std::vector<fs::path> paths;
	for (; current_idx < argc; current_idx++) {
		paths.push_back(argv[current_idx]);
	}

	// If we got no files or directories to look in we default to the current
	// working directory
	if (!paths.size()) {
		paths.push_back(".");
	}

	// This flag is true as long as all files were successfully replaced
	bool ok = true;
	for (auto &path : Path::list_files(paths, extensions)) {
		if (verbose) {
			std::cout << "Replacing " << path << std::endl;
		}

		if (replace_text(path, replacements)) {
			std::cerr << "Error while reading " << path << std::endl;
			ok = false;
		}
	}

	if (ok) {
		return 0;
	} else {
		return 5;
	}
}
