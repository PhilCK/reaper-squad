
/*
	ModelLoader.cpp
	---------------
	
	Project: Caffeine
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

// TODO: Make ModelData use smart ptrs.

#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Application/Settings.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <time.h>
#include <map>


namespace Caffeine {
namespace Systems {


struct ModelManager::Impl
{
	std::map<std::string, ModelData*>	loadedModels;
	CaffApp::Renderer &					renderer;
	bool								isHotLoading;
	
	explicit Impl(CaffApp::Renderer & renderer)
	: loadedModels()
	, isHotLoading(false)
	, renderer(renderer)
	{
	}
	
	~Impl()
	{
	}
	
	void addModel(const CaffApp::ModelLoaderXMLFile & modelSettings)
	{
		if(!modelSettings.name.empty() || !modelSettings.modelfilename.empty())
		{
			ModelData *data	= new ModelData;
			//memset(&data, 0, sizeof(data));
			
			data->xmlFileName	= modelSettings.originFilename;
			data->modelFileName = Utilities::GetPathDir() + "Assets/" + "Models/" +modelSettings.modelfilename;
			
			// Create the model
			{
				// TODO: Pass scale and rotation as local matrix.
				CaffApp::Model *model = new CaffApp::Model;
				if(!model->loadModel(data->modelFileName))
				{
					delete model;
					delete data;
					return;
				}
				
				data->model			= model;
			}
			
			// Timestamps
			{
				struct stat modelAttr;
				stat(data->modelFileName.c_str(), &modelAttr);
				data->modelTimeStamp = modelAttr.st_mtime;
				
				struct stat xmlAttr;
				stat(data->xmlFileName.c_str(), &xmlAttr);
				data->xmlTimeStamp = xmlAttr.st_mtime;
			}
						
			// Load up all the meshes.
			{
				const std::size_t noMeshes = data->model->getNumberOfMeshes();
				
				data->vertBufferIDs  = new CaffApp::VertexID[noMeshes];
				data->indexBufferIDs = new CaffApp::IndexID[noMeshes];
				
				for(unsigned int i = 0; i < noMeshes; ++i)
				{
					CaffApp::VertexContainer meshData = data->model->getMesh(i).getGLVertexBuffer();
					
					data->vertBufferIDs[i] = renderer.addVertexBuffer(&meshData[0], sizeof(float) * data->model->getMesh(i).getGLVertices(), CaffApp::BufferAccess::STATIC);
				}
			}
			
			// Check to see if it already exists
			if(loadedModels.find(modelSettings.name) != loadedModels.end())
			{
				ModelData *oldData = loadedModels[modelSettings.name];
				
				delete[] oldData->vertBufferIDs;
				delete[] oldData->indexBufferIDs;
				delete   oldData->model;
				delete   oldData;
				
				// TODO: Remove from renderer;
			}
			
			// Insert into map
			loadedModels[modelSettings.name] = data;
		}
	}
	
	void updateModels()
	{
		for(auto it = loadedModels.begin(); it != loadedModels.end(); ++it)
		{
			const ModelData *data = it->second;
			
			bool xmlUpdated		= false;
			bool modelUpdated	= false;
		
			// Get Current TimeStamps
			{
				struct stat modelAttr;
				stat(data->modelFileName.c_str(), &modelAttr);
				time_t modelLastUpdated = modelAttr.st_mtime;
				
				if(modelLastUpdated != data->modelTimeStamp) { modelUpdated = true; }
				
				struct stat xmlAttr;
				stat(data->xmlFileName.c_str(), &xmlAttr);
				time_t xmlLastUpdated = xmlAttr.st_mtime;
				
				if(xmlLastUpdated != data->xmlTimeStamp) { xmlUpdated = true; }
			}
			
			// Update data
			if(modelUpdated || xmlUpdated)
			{
				CaffApp::ModelLoaderXMLFile updateModelSettings(data->xmlFileName);
				addModel(updateModelSettings);
			}
		}
	}
	
}; // impl struct


ModelManager::ModelManager(const std::string & directory, const std::string & extension, const bool hotLoading, CaffApp::Renderer & renderer)
: m_pImpl(new Impl(renderer))
{
	m_pImpl->isHotLoading = hotLoading;
	std::string dirPath = Utilities::GetPathDir() + "Assets/" + directory;
	
	std::vector<std::string> filenames;
	
	Utilities::GetFileNamesOfTypeInDirectory(filenames, extension, dirPath);
	
	// Load all files
	for(auto it = filenames.begin(); it != filenames.end(); ++it)
	{
		CaffApp::ModelLoaderXMLFile modelSettings((*it));
		
		m_pImpl->addModel(modelSettings);
	}
}

ModelManager::~ModelManager()
{
}


CaffSys::ModelData & ModelManager::getModelData(const std::string & modelName)
{
	return *m_pImpl->loadedModels[modelName];
}


void ModelManager::checkForUpdates()
{
	// If models and hotloading then check for updates.
	if(!m_pImpl->loadedModels.empty() && m_pImpl->isHotLoading)
	{
		m_pImpl->updateModels();
	}
}


} // namespace
} // namespace
