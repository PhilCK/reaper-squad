
#include <Caffeine/Systems/TextureManager.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Application/Settings.hpp>
#include <time.h>
#include <map>

namespace Caffeine {
namespace Systems {

#pragma mark - Impl

struct TextureManager::Impl
{
	explicit Impl(CaffApp::Renderer & renderer, const bool hotLoading)
	: loadedTextures()
	, isHotLoading(hotLoading)
	, renderer(renderer)
	{
	}
		
	~Impl()
	{
	}
	
	void addTexture(const CaffApp::TextureLoaderXMLFile & settings)
	{
		if(!settings.name.empty() || !settings.texutreFilename.empty())
		{
			TextureData *data = new TextureData;
			
			data->xmlFileName	  = settings.originFilename;
			data->textureFilename = CaffUtil::GetPathDir() + "Assets/" + "Textures/" + settings.texutreFilename;
			
			// Timestamps
			{
				struct stat textureAttr;
				stat(data->textureFilename.c_str(), &textureAttr);
				data->textureTimeStamp = textureAttr.st_mtime;
				
				struct stat xmlAttr;
				stat(data->xmlFileName.c_str(), &xmlAttr);
				data->xmlTimeStamp = xmlAttr.st_mtime;
			}
			
			// Load up the texture
			{
				data->textureID = renderer.addTexture(data->textureFilename);
				
				if(data->textureID != CaffApp::NO_TEXTURE)
				{
					data->isLoaded = true;
				}
				else
				{
					delete data;
					return;
				}
			}
			
			// Check to see if it already exists
			if(loadedTextures.find(settings.name) != loadedTextures.end())
			{
				TextureData *oldData = loadedTextures[settings.name];
				
				// TODO: Remove from renderer;
			}

			// Insert into map
			loadedTextures[settings.name] = data;
		}
	} // addTexture
	

	void updateTextures()
	{
		for(auto it = loadedTextures.begin(); it != loadedTextures.end(); ++it)
		{
			const TextureData *data = it->second;
			
			bool xmlUpdated		= false;
			bool modelUpdated	= false;
		
			// Get Current TimeStamps
			{
				struct stat textureAttr;
				stat(data->textureFilename.c_str(), &textureAttr);
				time_t modelLastUpdated = textureAttr.st_mtime;
				
				if(modelLastUpdated != data->textureTimeStamp) { modelUpdated = true; }
				
				struct stat xmlAttr;
				stat(data->xmlFileName.c_str(), &xmlAttr);
				time_t xmlLastUpdated = xmlAttr.st_mtime;
				
				if(xmlLastUpdated != data->xmlTimeStamp) { xmlUpdated = true; }
			}
			
			// Update data
			if(modelUpdated || xmlUpdated)
			{
				CaffApp::TextureLoaderXMLFile updateTextureSettings(data->xmlFileName);
				addTexture(updateTextureSettings);
			}
		}
	} // updateTextures

	

	std::map<std::string, TextureData*>	loadedTextures;
	bool								isHotLoading;
	CaffApp::Renderer &					renderer;

}; // struct


#pragma mark - Texture Manager

TextureManager::TextureManager(const std::string & directory, const std::string & extension, const bool hotLoading, CaffApp::Renderer & renderer)
: m_pImpl(new Impl(renderer, hotLoading))
{
	m_pImpl->isHotLoading = hotLoading;
	std::string dirPath = Utilities::GetPathDir() + "Assets/" + directory;
	
	std::vector<std::string> filenames;
	
	Utilities::GetFileNamesOfTypeInDirectory(filenames, extension, dirPath);
	
	// Load all files
	for(auto it = filenames.begin(); it != filenames.end(); ++it)
	{
		CaffApp::TextureLoaderXMLFile settings((*it));
		
		m_pImpl->addTexture(settings);
	}
}


TextureManager::~TextureManager()
{
}


CaffSys::TextureData & TextureManager::getTextureData(const std::string &textureName)
{
	return *m_pImpl->loadedTextures[textureName];
}


void TextureManager::checkForUpdates()
{
	// If textures and hotloading then check for updates.
	if(!m_pImpl->loadedTextures.empty() && m_pImpl->isHotLoading)
	{
		m_pImpl->updateTextures();
	}
}


} // namespace
} // namespace
