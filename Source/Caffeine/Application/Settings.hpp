
#ifndef CAFFEINE_APPLICATION_SETTINGS_INCLUDED
#define CAFFEINE_APPLICATION_SETTINGS_INCLUDED

#include <Caffeine/Utilities/Directories.hpp>
#include <tinyxml2/tinyxml2.h>
#include <string>
#include <map>
#include <algorithm>
#include <array>


namespace Caffeine {
namespace Application {

    
struct XMLFile
{
	XMLFile(const std::string filename, const std::string node);
	XMLFile(tinyxml2::XMLElement* element, const std::string node);
	
	tinyxml2::XMLElement *currentNode;
	tinyxml2::XMLDocument xmlFile;
	std::string nodeName;
};


#pragma mark - Application Settings

struct AppXML : public XMLFile
{
	explicit AppXML();

	// Window Settings
	struct WindowXML : public XMLFile
	{
		explicit WindowXML(tinyxml2::XMLElement *parentNode);
	
		unsigned int width;
		unsigned int height;
		bool isFullscreen;
		std::string title;
		
	} windowDetails;
	
	
	// Render Settings
	struct RendererXML : public XMLFile
	{
		explicit RendererXML(tinyxml2::XMLElement *element);
		
		bool		 isVSync;
		unsigned int multisamples;
		unsigned int depthSize;
	
		struct ColorBufferXML : public XMLFile
		{
			explicit ColorBufferXML(tinyxml2::XMLElement *element);
		
			unsigned int redBits;
			unsigned int greenBits;
			unsigned int blueBits;
			unsigned int alphaBits;
			
		} colorBufferDetails;
		
	} rendererDetails;
	
	// System Settings
	struct SystemsXML : public XMLFile
	{
	
		explicit SystemsXML(tinyxml2::XMLElement *element);
	
		struct StatesXML : public XMLFile
		{
			explicit StatesXML(tinyxml2::XMLElement *element);
		
			std::string startState;
			
		} statesDetails;
	
	} systemsDetails;
	
	// Model Settings
	struct ModelsXML : public XMLFile
	{
		explicit ModelsXML(tinyxml2::XMLElement *element);
		
		std::string modelsDir;
		std::string modelsExt;
		bool		hotLoading;
		
	} modelDetails;
	
	// Texture Settings
	struct TexturesXML : public XMLFile
	{
		explicit TexturesXML(tinyxml2::XMLElement *element);
		
		std::string texturesDir;
		std::string texturesExt;
		bool		hotLoading;
		
	} textureDetails;
	
	// Entity
//	struct EntityXML : public XMLFile
//	{
//		explicit EntityXML(tinyxml2::XMLElement *element);
//		
//		std::string entityDir;
//		std::string entityExt;
//		bool		hotLoading;
//		
//	} entityDetails;
//	

};


#pragma mark - Model Load Settings

struct ModelLoaderXMLFile : public XMLFile
{
	explicit		ModelLoaderXMLFile(const std::string & filename);
	
	std::string		name;
	std::string		modelfilename;
	
	float			scale_x;
	float			scale_y;
	float			scale_z;
	
	float			rot_x;
	float			rot_y;
	float			rot_z;
	
	
	std::string		originFilename; // This is the xml file itself, not an xml setting
};


#pragma mark - Texture Load Settings

struct TextureLoaderXMLFile : public XMLFile
{
	explicit		TextureLoaderXMLFile(const std::string & filename);
	
	std::string		name;
	std::string		texutreFilename;
	
	
	std::string		originFilename; // This is the xml file itself, not an xml setting
	
}; // struct


#pragma mark - Entity;


struct ComponentXML
{
	
};

struct EntityXML
{
	std::string					name;
	bool						isVisible;

	std::vector<EntityXML>		children;
	std::vector<ComponentXML>	components;
};

struct EntityLoaderXMLFile : public XMLFile
{
	explicit EntityLoaderXMLFile(const std::string & filename);
	
	EntityXML	entity;

}; // struct


} // namespace
} // namespace

#endif // include guard
