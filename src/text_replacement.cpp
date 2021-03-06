#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "path.h"
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


int replace_text(fs::path const &src_path,
	std::vector<std::pair<std::string, std::string>> repl_pairs
) {
	std::fstream src_file, dst_file;
	src_file.open(src_path.string());

	fs::path dst_path(Path::open_tmp_file("/tmp", dst_file));

	if (src_file.is_open() && dst_file.is_open()) {
		std::string line;
		while (getline(src_file, line)) {
			Line current(line);
			current.replace_all(repl_pairs);
			dst_file << current.get_new_line() << std::endl;
		}

		src_file.close();
		dst_file.close();
		std::rename(dst_path.c_str(), src_path.c_str());
		return 0;
	} else {
		if (src_file.is_open()) {
			src_file.close();
		}
		if (dst_file.is_open()) {
			dst_file.close();
		}
		return 1;
	}
}

void Line::replace(std::pair<std::string, std::string> repl) {
	size_t index;
	char c = '\0';
	std::string word = std::string((repl.second).length(),c);

	while ((index = (this->old_line).find(repl.first)) != std::string::npos){
		this->old_line = (this->old_line).replace(index,
			(repl.first).length(), word);
		this->new_line = (this->new_line).replace(index,
			(repl.first).length(), repl.second);
	}
}

void Line::replace_all(std::vector<std::pair<std::string, std::string>> repl_pairs) {
	for (int m = 0; m < repl_pairs.size(); m++){
		this->replace(repl_pairs[m]);
	}
}


std::string Line::get_old_line() {
	return this->old_line;
}


std::string Line::get_new_line() {
	return this->new_line;
}


Line::Line(std::string line) {
	this->new_line = line;
	this->old_line = line;
}
