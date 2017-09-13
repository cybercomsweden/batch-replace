#include <doctest.h>

#include "utils.h"

TEST_CASE("Test human_join_or") {
	std::vector<std::string> parts;

	CHECK(human_join_or(parts) == "");

	parts.push_back("foo");
	CHECK(human_join_or(parts) == "foo");

	parts.push_back("bar");
	CHECK(human_join_or(parts) == "foo or bar");

	parts.push_back("biz");
	parts.push_back("baz");
	CHECK(human_join_or(parts) == "foo, bar, biz or baz");
}
