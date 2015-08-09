
#ifndef CAFFEINE_UTILITIES_DIRECTORIES_INCLUDED
#define CAFFEINE_UTILITIES_DIRECTORIES_INCLUDED

#include <string>
#include <vector>

namespace Caffeine {
namespace Utilities {
  
	std::string GetPathDir();

	void GetFileNamesOfTypeInDirectory(std::vector<std::string> & outVec, const std::string & ext, const std::string & directory);
  
} // namespace
} // namespace

#endif // include guard