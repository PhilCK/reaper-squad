
#include <Caffeine/Utilities/XMLHelpers.hpp>
#include <Caffeine/Utilities/Logging.hpp>

namespace Caffeine {
namespace Utilities {

void XMLFindBool(bool *result, const std::string name, const tinyxml2::XMLElement* element)
{
	if(element)
	{
		const tinyxml2::XMLElement *findElement = element->FirstChildElement(name.c_str());
		
		if(findElement)
		{
			std::string resultStr = findElement->GetText();
			std::transform(resultStr.begin(), resultStr.end(), resultStr.begin(), ::tolower);
			
			if(resultStr == "true") {
				*result = true;
			} else {
				*result = false;
			}
		}
		else
		{
			std::string parentName = element->Value();
			std::string msg = "No value for " + name + " in " + parentName + ".";
		
			LOADING_LOG(msg);
		}
	}
}

void XMLFindUint(unsigned int *result, const std::string name, tinyxml2::XMLElement* element)
{
	if(element)
	{
		tinyxml2::XMLElement *findElement = element->FirstChildElement(name.c_str());
		
		if(findElement)
		{
			std::string resultStr = findElement->GetText();
			*result = ::atoi(resultStr.c_str());
		}
		else
		{
			std::string parentName = element->Value();
			std::string msg = "No value for " + name + " in " + parentName + ".";
		
			LOADING_LOG(msg);
		}
	}
}

void XMLFindFloat(float *result, const std::string name, tinyxml2::XMLElement* element)
{
	if(element)
	{
		tinyxml2::XMLElement *findElement = element->FirstChildElement(name.c_str());
		
		if(findElement)
		{
			std::string resultStr = findElement->GetText();
			*result = ::atof(resultStr.c_str());
		}
		else
		{
			std::string parentName = element->Value();
			std::string msg = "No value for " + name + " in " + parentName + ".";
		
			LOADING_LOG(msg);
		}
	}
}

void XMLFindStdString(std::string *result, const std::string name, const tinyxml2::XMLElement* element)
{
	if(element)
	{
		const tinyxml2::XMLElement *findElement = element->FirstChildElement(name.c_str());
		
		if(findElement)
		{
			std::string resultStr = findElement->GetText();
			*result = resultStr;
		}
		else
		{
			std::string parentName = element->Value();
			std::string msg = "No value for " + name + " in " + parentName + ".";
		
			LOADING_LOG(msg);
		}
	}
}


void XMLGetTokens(std::string &tokenStr, tinyxml2::XMLElement *root, const std::string &elementName, const std::string findAttrName)
{
	const tinyxml2::XMLElement *param = root->FirstChildElement(elementName.c_str());
	
	//TODO: Is there a better way to do this without looping each time?
	while(param != nullptr)
	{
		const std::string attrName = param->FindAttribute("name")->Value();
		
		if(attrName == findAttrName)
		{
			tokenStr += param->GetText();
			tokenStr += " ";
		}
		
		param = param->NextSiblingElement();
	}
}


} // namespace
} // namespace
