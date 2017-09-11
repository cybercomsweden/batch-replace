#include <doctest.h>

#include "path.h"

TEST_CASE("Test has_extension") {
	CHECK(Path::has_extension("foo.bar", ".bar"));
	CHECK(!Path::has_extension("foo.txt", ".bar"));
	CHECK(Path::has_extension("foo", ""));
}
 