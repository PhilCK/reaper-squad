

#include <Caffeine/Application/Settings.hpp>
#include <Caffeine/Utilities/Logging.hpp>

#pragma mark - XML Helpers

namespace
{
	
	void XMLFindBool(bool *result, const std::string name, tinyxml2::XMLElement* element)
	{
		if(element)
		{
			tinyxml2::XMLElement *findElement = element->FirstChildElement(name.c_str());
			
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
	
	void XMLFindStdString(std::string *result, const std::string name, tinyxml2::XMLElement* element)
	{
		if(element)
		{
			tinyxml2::XMLElement *findElement = element->FirstChildElement(name.c_str());
			
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

}



namespace Caffeine {
namespace Application {

#pragma mark - XMLFile

XMLFile::XMLFile(const std::string filename, const std::string node)
: currentNode(0)
{
	xmlFile.LoadFile(filename.c_str());
	
	if(!xmlFile.Error()) {
		LOADING_LOG("Loaded " + filename + " settings.")
	} else {
		LOADING_LOG("Can't find " + filename + " settings. ")
	}
	
	currentNode = xmlFile.FirstChildElement(node.c_str());
}

XMLFile::XMLFile(tinyxml2::XMLElement* element, const std::string node)
: currentNode(0)
{
	if(element)
	{
		currentNode = element->FirstChildElement(node.c_str());
	}
}


#pragma mark - AppXML

AppXML::AppXML()
: XMLFile(Utilities::GetPathDir() + "Assets/Application.xml", "application")
, windowDetails(currentNode)
, rendererDetails(currentNode)
, systemsDetails(currentNode)
, modelDetails(currentNode)
, textureDetails(currentNode)
{
	LOG_LINE_SEPERATOR
}


AppXML::WindowXML::WindowXML(tinyxml2::XMLElement* parentNode)
: XMLFile(parentNode, "window")
, width(800)
, height(600)
, isFullscreen(false)
, title("Give me MOAR CAFFEINE!!!")
{
	XMLFindUint(&width,			"width",		currentNode);
	XMLFindUint(&height,		"height",		currentNode);
	XMLFindBool(&isFullscreen,	"fullscreen",	currentNode);
	XMLFindStdString(&title	,	"app_title",	currentNode);
}


AppXML::RendererXML::RendererXML(tinyxml2::XMLElement* parentNode)
: XMLFile(parentNode, "renderer")
, isVSync(true)
, multisamples(0)
, depthSize(24)
, colorBufferDetails(currentNode)
{
	XMLFindBool(&isVSync,		"v_sync",		currentNode);
	XMLFindUint(&multisamples,	"multisamples", currentNode);
	XMLFindUint(&depthSize,		"depth_size",	currentNode);
}


AppXML::RendererXML::ColorBufferXML::ColorBufferXML(tinyxml2::XMLElement* parentNode)
: XMLFile(parentNode, "color_buffer")
, redBits(3)
, greenBits(3)
, blueBits(2)
, alphaBits(0)
{
	XMLFindUint(&redBits,	"red_bits",	  currentNode);
	XMLFindUint(&greenBits, "green_bits", currentNode);
	XMLFindUint(&blueBits,	"blue_bits",  currentNode);
	XMLFindUint(&alphaBits, "alpha_bits", currentNode);
}


AppXML::SystemsXML::SystemsXML(tinyxml2::XMLElement* parentNode)
: XMLFile(parentNode, "systems")
, statesDetails(currentNode)
{
}


AppXML::SystemsXML::StatesXML::StatesXML(tinyxml2::XMLElement* parentNode)
: XMLFile(parentNode, "states")
, startState("")
{
	XMLFindStdString(&startState, "start_state", currentNode);
}


AppXML::ModelsXML::ModelsXML(tinyxml2::XMLElement *parentNode)
: XMLFile(parentNode, "models")
, modelsDir("")
, modelsExt("model")
, hotLoading(true)
{
	XMLFindStdString(&modelsDir, "directory",	currentNode);
	XMLFindStdString(&modelsExt, "extension",	currentNode);
	XMLFindBool(&hotLoading,	 "hot_loading", currentNode);
}


AppXML::TexturesXML::TexturesXML(tinyxml2::XMLElement *parentNode)
: XMLFile(parentNode, "textures")
, texturesDir("")
, texturesExt("textures")
, hotLoading(true)
{
	XMLFindStdString(&texturesDir,	"directory",	currentNode);
	XMLFindStdString(&texturesExt,	"extension",	currentNode);
	XMLFindBool(&hotLoading,		"hot_loading",	currentNode);
}

#pragma mark - ModelLoaderXMLFile

ModelLoaderXMLFile::ModelLoaderXMLFile(const std::string & filename)
: XMLFile(filename, "model")
, originFilename(filename)
, name("")
, modelfilename("")
, scale_x(1)
, scale_y(1)
, scale_z(1)
, rot_x(0)
, rot_y(0)
, rot_z(0)
{
	XMLFindStdString(&name,			"name",			currentNode);
	XMLFindStdString(&modelfilename,"filename",		currentNode);
	XMLFindFloat(&scale_x,			"scale_x",		currentNode);
	XMLFindFloat(&scale_y,			"scale_y",		currentNode);
	XMLFindFloat(&scale_z,			"scale_z",		currentNode);
	XMLFindFloat(&rot_x,			"rotation_x",	currentNode);
	XMLFindFloat(&rot_y,			"rotation_y",	currentNode);
	XMLFindFloat(&rot_z,			"rotation_z",	currentNode);
	
	LOG_LINE_SEPERATOR
}



#pragma mark - TextureLoaderXMLFile

TextureLoaderXMLFile::TextureLoaderXMLFile(const std::string & filename)
: XMLFile(filename, "texture")
, name("")
, texutreFilename("")
{
	XMLFindStdString(&name,				"name",		currentNode);
	XMLFindStdString(&texutreFilename,	"filename", currentNode);
	
	LOG_LINE_SEPERATOR
}


} // namespace
} // namespace
