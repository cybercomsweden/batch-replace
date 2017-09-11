#include <algorithm>
#include <string>

#include "path.h"

namespace Path {

std::string basename(std::string const& pathname) {
	return std::string(
		std::find_if(
			pathname.rbegin(), pathname.rend(), MatchPathSeparator()).base(),
		pathname.end());
}

}
