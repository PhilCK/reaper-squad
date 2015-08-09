
#ifndef CAFFEINE_UTILITIES_XML_HELPERS_INCLUDED
#define CAFFEINE_UTILITIES_XML_HELPERS_INCLUDED

#include <Caffeine/Platform.hpp>
#include <tinyxml2/tinyxml2.h>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

/*
	Helpers for TinyXML.
	http://www.grinninglizard.com/tinyxml2/
	
	
*/

namespace Caffeine {
namespace Utilities {

// First paramater is the result, name is the attribute name, and element the tinyxml element.

// Gets a bool from the xml that is in the format of <Attr>true</Attr> or <Attr>false</Attr>.
void XMLFindBool(bool *result, const std::string name, const tinyxml2::XMLElement *element);

// Gets a uint from the xml that is in the format of <Attr>123</Attr>.
void XMLFindUint(unsigned int *result, const std::string name, tinyxml2::XMLElement *element);

// Gets a float from the xml that is in the format of <Attr>1.23</Attr>.
void XMLFindFloat(float *result, const std::string name, tinyxml2::XMLElement *element);

// Gets a std::string from the xml that is in the format of <Attr>String</Attr>.
void XMLFindStdString(std::string *result, const std::string name, const tinyxml2::XMLElement *element);


void XMLGetTokens(std::string &tokenStr, tinyxml2::XMLElement *root, const std::string &elementName, const std::string findAttrName);

//typedef void(*ParamHelper)(const std::string &, const tinyxml2::XMLElement *, void*);

//inline void ParamVector3(const std::string &item, const tinyxml2::XMLElement *element, void* vec3)
//{
//	std::string value = element->GetText();
//	
//	std::istringstream ss(value);
//	std::istream_iterator<std::string> begin(ss), end;
//	std::vector<std::string>objTokens(begin, end);
//	
//	float* outputVec = static_cast<float*>(vec3);
//	
//	float x, y, z;
//	
//	if(objTokens.size() > 0)
//	{
//		x = ::atof(objTokens[0].c_str());
//		
//		outputVec[0] = x;
//		outputVec[1] = x;
//		outputVec[2] = x;
//	}
//	
//	if(objTokens.size() > 1)
//	{
//		y = ::atof(objTokens[1].c_str());
//		
//		outputVec[1] = y;
//		outputVec[2] = y;
//	}
//	
//	if(objTokens.size() > 2)
//	{
//		z = ::atof(objTokens[2].c_str());
//
//		outputVec[2] = z;
//	}
//}


} // namespace
} // namespace

#endif // include guard
