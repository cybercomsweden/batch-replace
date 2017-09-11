#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "text_replacement.h"

namespace fs = std::experimental::filesystem;

struct CompareReplacements {
	bool operator()(
			std::pair<std::string, std::string> const &left,
			std::pair<std::string, std::string> const &right
	) {
		return left.first.size() > right.first.size();
	}
};

std::vector<std::pair<std::string, std::string>>
replacements_from_file(fs::path const &path) {
	std::vector<std::pair<std::string, std::string>> replacements;

	std::fstream file;
	file.open(path.string());

	std::string from;
	std::string to;
	while ((file >> from) && (file >> to)) {
		replacements.push_back(std::make_pair(from, to));
	}

	file.close();

	// Sort the from replacements in descending order by size since we must
	// replace longer chunks before shorter
	CompareReplacements comparator;
	std::sort(replacements.begin(), replacements.end(), comparator);

	return replacements;
}
