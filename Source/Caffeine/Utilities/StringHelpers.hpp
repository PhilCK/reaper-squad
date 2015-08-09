
#ifndef CAFFEINE_UTILITIES_STRING_HELPERS_INCLUDED
#define CAFFEINE_UTILITIES_STRING_HELPERS_INCLUDED

#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <glm/glm.hpp>

namespace Caffeine {
namespace Utilities {

// Will split a string based on spaces. ie "foo bar baz" would become ["foo", "bar", "baz"]
inline std::vector<std::string> SplitStringBySpaces(const std::string &tokenString)
{
	std::istringstream ss(tokenString);
	std::istream_iterator<std::string> begin(ss), end;
	std::vector<std::string>tokens(begin, end);
	
	return tokens;
}


// Converts string items to a vec3.
inline glm::vec3 MakeVec3(const std::string &x, const std::string &y, const std::string &z)
{
	glm::vec3 result(::atof(x.c_str()), ::atof(y.c_str()), ::atof(z.c_str()));
	
	return result;
}


} // namespace
} // namespace

#endif // namespace
