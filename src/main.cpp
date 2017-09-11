#include <iostream>
#include <string>
#include <vector>

#include "path.h"


void print_help(std::string const &name) {
	std::cerr << Path::basename(name) << " [options] <replacement-file> [files..]" << std::endl;
	std::cerr << std::endl;

	std::cerr << "Batch replace allows you to perform advanced search and replace on arbitrary" << std::endl;
	std::cerr << "text files. It allows " << std::endl;
	std::cerr << std::endl;

	std::cerr << "Arguments:" << std::endl;
	std::cerr << "    <replacement-file>  A text file containing things to replace. If the" << std::endl;
	std::cerr << "                        file name starts with a hyphen (-) use -- as the." << std::endl;
	std::cerr << "                        last option." << std::endl;
	std::cerr << "    [files..]           Optional list of files and directories to perform" << std::endl;
	std::cerr << "                        replacements in. File extentions can be specified" << std::endl;
	std::cerr << "                        using --exts." << std::endl;
	std::cerr << std::endl;

	std::cerr << "Options:" << std::endl;
	std::cerr << "    -h, --help  Display this help message" << std::endl;
	std::cerr << "    --exts      Comma separated list of extensions to search in." << std::endl;
}


int main(int argc, char* argv[]) {
	// Parse all options
	bool help = false;
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
			print_help(argv[0]);
			return 0;
		} else if (option == "--exts") {
			// We need to consume the next arg, and verify that we don't read
			// out of range
			current_idx++;
			if (current_idx >= argc) {
				std::cerr << "No extensions provided to --exts" << std::endl;
				return 3;
			}
			std::string exts(argv[current_idx]);
			std::cout << "Got exts: " << exts << std::endl;
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

	std::string replacements_file(argv[current_idx++]);
	std::cout << "Replacement file is: " << replacements_file << std::endl;

	// Check if there where any files or folders specified
	std::vector<std::string> paths;
	for (; current_idx < argc; current_idx++) {
		paths.push_back(argv[current_idx]);
	}

	// If we got no files or directories to look in we default to the current
	// working directory
	if (!paths.size()) {
		paths.push_back(".");
	}

	std::cout << "Looking in paths:" << std::endl;
	for (auto &path : paths) {
		std::cout << "    " << path << std::endl;
	}
}
