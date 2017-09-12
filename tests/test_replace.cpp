#include <doctest.h>
#include <string>
#include <utility>
#include <vector>

#include "text_replacement.h"

TEST_CASE("Test Line::replace_all") {
	Line line("foo bar foobar biz baz");

	std::vector<std::pair<std::string, std::string>> replacements;
	replacements.push_back(std::make_pair("foobar", "bizbaz"));
	replacements.push_back(std::make_pair("foo", "bar"));
	replacements.push_back(std::make_pair("bar", "foo"));

	line.replace_all(replacements);

	CHECK(line.get_new_line() == "bar foo bizbaz biz baz");
}
