
#import <Caffeine/Utilities/Directories.hpp>
#import <Caffeine/Utilities/Logging.hpp>
#import <dirent.h>
#import <Foundation/Foundation.h>

namespace Caffeine {
namespace Utilities {

std::string GetPathDir()
{
	static std::string path;

	if(path.empty())
	{
		NSBundle *bundle = [NSBundle mainBundle];
		
		if(bundle == nil) {
			return "";
		}
		else
		{
			NSString *rPath = [bundle resourcePath];
			path = [rPath UTF8String] + std::string("/");
		}
	}
	
	return path;
}



void GetFileNamesOfTypeInDirectory(std::vector<std::string> & outVec, const std::string & ext, const std::string & directory)
{
	DIR *dir;
	struct dirent *ent;
	
	if((dir = opendir(directory.c_str())) != nullptr)
	{
		while((ent = readdir(dir)) != nullptr)
		{
			if(ent->d_type == DT_REG)
			{
				std::string file(ent->d_name);
				
				if(file.find("." + ext) != std::string::npos)
				{
					outVec.push_back(directory + file);
				}
			}
		}
		
		closedir(dir);
	}
	
	else
	{
		DEBUG_LOG("Failed opening dir.")
	}

}


} // namespace
} // namespace
