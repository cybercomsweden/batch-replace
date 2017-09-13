#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "path.h"
#include "text_replacement.h"
#include "utils.h"

namespace fs = std::experimental::filesystem;

void print_help(std::string const &exec_name) {
	std::cerr
		<< exec_name << " [options] <replacement-file> [files..]" << std::endl
		<< std::endl

		<< "Batch replace allows you to perform advanced search and replace on arbitrary" << std::endl
		<< "text files." << std::endl
		<< std::endl

		<< "Arguments:" << std::endl
		<< "    <replacement-file>  A text file containing things to replace. If the" << std::endl
		<< "                        file name starts with a hyphen (-) use -- as the." << std::endl
		<< "                        last option." << std::endl
		<< "    [files..]           Optional list of files and directories to perform" << std::endl
		<< "                        replacements in. File extentions can be specified" << std::endl
		<< "                        using --ext." << std::endl
		<< std::endl

		<< "Options:" << std::endl
		<< "    -e, --ext      Extensions to search for. For multiple extentions, " << std::endl
		<< "                   use -e or --ext for each one." << std::endl
		<< "    -h, --help     Display this help message." << std::endl
		<< "    -v, --verbose  Display information on what the program is doing." << std::endl
		<< std::endl

		<< "Replacement file format:" << std::endl
		<< "    The replacement file contains rows of pairs of replacements:" << std::endl
		<< std::endl
		<< "        replace-this" << std::endl
		<< "        with-this" << std::endl
		<< std::endl
		<< "        change-this" << std::endl
		<< "        to-this" << std::endl
		<< std::endl
		<< "    The replacement file example above would 'replace-this' to 'with-this'" << std::endl
		<< "    and all 'change-this' to 'to-this'." << std::endl
		<< std::endl

		<< "Example usage:" << std::endl
		<< "    # Replace in all .txt files in the current working directory" << std::endl
		<< "    " << exec_name << " -e txt replacements.txt" << std::endl
		<< "    # Replace in all file in directory a/" << std::endl
		<< "    " << exec_name << " replacements.txt a/" << std::endl
		<< std::endl
		<< "    # Replace in all .cpp and .h files in directories a/ and b/" << std::endl
		<< "    " << exec_name << " --ext cpp --ext h replacements.txt a/ b/" << std::endl
		<< std::endl;
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
			std::string ext = ".";
			ext += std::string(argv[current_idx]);
			extensions.push_back(ext);
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

	// Print which file extensions we're looking at
	if (verbose) {
		if (extensions.size()) {
			std::cout << "Looking for files ending with " << human_join_or(extensions) << std::endl;
		} else {
			std::cout << "Looking for files with any extension" << std::endl;
		}
	}

	// This flag is true as long as all files were successfully replaced
	bool ok = true;
	for (auto &path : Path::list_files(paths, extensions)) {
		if (verbose) {
			std::cout << "Replacing " << path << std::endl;
		}

		if (replace_text(path, replacements)) {
			std::cerr << "Couldn't read or write " << path << std::endl;
			ok = false;
		}
	}

	if (ok) {
		return 0;
	} else {
		return 5;
	}
}
