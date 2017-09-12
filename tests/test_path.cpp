#include <doctest.h>
#include <string>
#include <vector>

#include "path.h"

TEST_CASE("Test has_extension") {
	CHECK(Path::has_extension("foo.bar", ".bar"));
	CHECK(!Path::has_extension("foo.txt", ".bar"));
	CHECK(Path::has_extension("foo", ""));
}

TEST_CASE("Test has_valid_extension") {
	std::vector<std::string> extensions;
	extensions.push_back(".txt");
	extensions.push_back(".h");
	extensions.push_back(".cpp");

	CHECK(Path::has_valid_extension("test.txt", extensions));
	CHECK(Path::has_valid_extension("test.h", extensions));
	CHECK(Path::has_valid_extension("test.cpp", extensions));
	CHECK(!Path::has_valid_extension("test.cc", extensions));
	CHECK(!Path::has_valid_extension("test.hpp", extensions));
	CHECK(!Path::has_valid_extension("test.py", extensions));
}
