#include <string>
#include <vector>

#include "utils.h"

std::string human_join_or(std::vector<std::string> const &parts) {
	std::string output;
	for (int i = 0, n = parts.size(); i < n; i++) {
		if (i != 0 && i == n - 1) {
			output += " or ";
		} else if (i != 0) {
			output += ", ";
		}
		output += parts[i];
	}
	return output;
}
