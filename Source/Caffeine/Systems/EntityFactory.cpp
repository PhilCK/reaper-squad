
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Utilities/Directories.hpp>
#include <Caffeine/Utilities/XMLHelpers.hpp>
#include <Caffeine/Utilities/Logging.hpp>
#include <string>
#include <iostream>

namespace {
	
	// XML Identifiers
	const std::string xmlRoot				= "entity";
	const std::string xmlEntityName			= "name";
	const std::string xmlEntityVisible		= "visible";
	const std::string xmlEntityComponents	= "components";
	const std::string xmlComponent			= "component";
	const std::string xmlComponentName		= "name";
	const std::string xmlChildren			= "child_entities";
	const std::string xmlChild				= "entity";
	const std::string xmlParams				= "params";
	
	
	bool GetEntityFromXML(const tinyxml2::XMLElement *root, CaffSys::EntityDetails& entityDetails)
	{
		if(!root)
		{
			return false;
		}
		
		entityDetails.name		= "none";
		entityDetails.isVisible	= true;
		
		// Name and isVisible
		{
			CaffUtil::XMLFindStdString(&entityDetails.name, "name",		root);
			CaffUtil::XMLFindBool(&entityDetails.isVisible,	"visible",	root);
			
			// Fail on no name, if name already registered.
			if(entityDetails.name == "none")
			{
				return false;
			}
		}
		
		// Components
		{
			const tinyxml2::XMLElement *components = root->FirstChildElement(xmlEntityComponents.c_str());
			
			if(components)
			{
				const tinyxml2::XMLElement *component  = components->FirstChildElement(xmlComponent.c_str());
				
				while(component != nullptr)
				{
					CaffSys::EntityComponentDetails componentDetails;
					
					CaffUtil::XMLFindStdString(&componentDetails.name, "name", component);
					
					const tinyxml2::XMLElement *params = component->FirstChildElement(xmlParams.c_str());
					if(params != nullptr)
					{
						tinyxml2::XMLPrinter printer;
						params->Accept(&printer);
						
						componentDetails.params = printer.CStr();
					}
					
					if(componentDetails.name != "")
					{
						entityDetails.components.push_back(componentDetails);
					}
					
					component = component->NextSiblingElement(xmlComponent.c_str());
				}
			} // if(components)
		}
		
		// Children
		{
			const tinyxml2::XMLElement *children = root->FirstChildElement(xmlChildren.c_str());
			
			if(children)
			{
				const tinyxml2::XMLElement *child = children->FirstChildElement(xmlChild.c_str());
				
				while(child != nullptr)
				{
					CaffSys::EntityDetails childDetails;
					
					bool success = GetEntityFromXML(child, childDetails);
					
					if(success)
					{
						entityDetails.children.push_back(childDetails);
					}
				
					child = child->NextSiblingElement(xmlChild.c_str());
				}
			} // if(children)
			
		}
		
		return true;
	} // bool GetEntityFromXML(...)
	
	
	std::unique_ptr<CaffEnt::Entity> InstantiateEntity(CaffSys::EntityDetails details, const std::map<std::string, const CaffSys::ComponentFactory*> &components)
	{
		CaffEnt::EntityUniquePtr entity(new CaffEnt::Entity(details.name));

		// Components
		{
			auto it  = details.components.begin();
			auto end = details.components.end();
			
			for(; it != end; ++it)
			{
				if(components.count(it->name))
				{
					const CaffSys::ComponentFactory *factory = components.at(it->name);
					
					CaffEnt::ComponentUniquePtr component(factory->create());
					
					const std::string params = (*it).params;
					
					if(params != "")
					{
						entity->addComponent(component, (*it).params);
					}
					else
					{
						entity->addComponent(component);
					}
				}
			}
		}
		
		// Children
		{
			auto it  = details.children.begin();
			auto end = details.children.end();
			
			for(; it != end; ++it)
			{
				CaffEnt::EntityUniquePtr child = InstantiateEntity(*it, components);
				
				if(child)
				{
					entity->addChild(child);
				}
			}
		}
		
		return entity;
	}
}


namespace Caffeine {
namespace Systems {

EntityFactory::EntityFactory()
: m_registeredEntities()
, m_registeredComponentFactories()
{
	checkForUpdates();

	// Register the default components.
	registerComponent("Transform",  new TransformComponentFactory);
	registerComponent("Physics",	new PhysicsComponentFactory);
}


EntityFactory::~EntityFactory()
{
	clearFactories();
}


void EntityFactory::checkForUpdates()
{
	// Get all the entities in the directory.
	const std::string dirPath = Utilities::GetPathDir() + "Assets/Entities/";
	
	std::vector<std::string> filenames;
	
	Utilities::GetFileNamesOfTypeInDirectory(filenames, "xml", dirPath);
	
	// Load all files
	for(auto it = filenames.begin(); it != filenames.end(); ++it)
	{
		// Load file
		tinyxml2::XMLDocument doc;
		doc.LoadFile(it->c_str());

		// If doc not valid.
		if(doc.Error())
		{
			std::string msg = "Entity Desc: " + (*it) + " not valid!";
			DEBUG_LOG(msg)
			continue;
		}
		
		// Root Element
		tinyxml2::XMLElement *root;
		root = doc.FirstChildElement("entity");
		
		// If no root try next one.
		if(!root)
		{
			std::string msg = "Entity Desc: " + (*it) + " no root found!";
			DEBUG_LOG(msg)
			continue;
		}
		
		EntityDetails details;
		
		bool success = GetEntityFromXML(root, details);
		
		if(success)
		{
			// TODO: Double check name isn't already in existance.
			m_registeredEntities[details.name] = details;
		}
		else
		{
			std::string msg = "Failed to add Entity Desc: " + (*it);
			DEBUG_LOG(msg);
		}
	}
}


std::unique_ptr<CaffEnt::Entity> EntityFactory::createInstance(const std::string & id)
{
	if(m_registeredEntities.count(id))
	{
		const EntityDetails &details = m_registeredEntities.at(id);
		
		return InstantiateEntity(details, m_registeredComponentFactories);
	}
	
	return Entity::EntityUniquePtr();
}


bool EntityFactory::registerComponent(const std::string & factoryName, ComponentFactory *factory)
{
	if(!m_registeredComponentFactories.count(factoryName))
	{
		m_registeredComponentFactories[factoryName] = factory;
		return true;
	}

	return false;
}


void EntityFactory::clearFactories()
{
	auto it = m_registeredComponentFactories.begin();
	auto end = m_registeredComponentFactories.end();
	
	for(; it != end; ++it)
	{
		delete it->second;
	}
	
	m_registeredComponentFactories.clear();
}


# pragma mark - Factorys for system components.

std::unique_ptr<Entity::Component> TransformComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::TransformComponent());
}

std::unique_ptr<Entity::Component> PhysicsComponentFactory::create() const {
	return Entity::ComponentUniquePtr(new Components::PhysicsComponent());
}


} // namespace
} // namespace
