#ifndef PATH_H
#define PATH_H

namespace Path {

// TODO: Support other platforms than UNIX/Linux
struct MatchPathSeparator
{
	bool operator()(char ch) const {
		return ch == '/';
	}
};


std::string basename(std::string const&);


}

#endif
