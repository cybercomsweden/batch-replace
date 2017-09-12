#ifndef REPLACE_H
#define REPLACE_H

#include <experimental/filesystem>
#include <string>
#include <utility>
#include <vector>

namespace fs = std::experimental::filesystem;

class Line
{
	std::string new_line;
	std::string old_line;
public:
	std::string get_new_line();
	std::string get_old_line();
	void replace(std::pair<std::string, std::string>);
	void replace_all(std::vector<std::pair<std::string, std::string>>);
	Line(std::string);
};


std::vector<std::pair<std::string, std::string>> replacements_from_file(fs::path const &);


int replace_text(fs::path const &, std::vector<std::pair<std::string, std::string>>);

#endif
