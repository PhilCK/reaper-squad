
#ifndef CAFFEINE_SYSTEMS_ENTITY_FACTORY_INCLUDED
#define CAFFEINE_SYSTEMS_ENTITY_FACTORY_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <tinyxml2/tinyxml2.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

// Helper Macros for defining component factories. Enter the name of component
// which will produce a factory with the same name and Factory appended at the end.
// ie. COMPONENT_FACTORY_HEADER(FooComponent) = FooComponentFactory
// Warning: breaking this convention will lead to issues as name is used to identify
// The component as well as create the name.
// This one is for the interface of the factory.
#define COMPONENT_FACTORY_INTERFACE(name)					\
	class name##Factory : public CaffSys::ComponentFactory	\
	{														\
	public:													\
		explicit name##Factory() {}							\
				 ~name##Factory() {}						\
															\
		std::unique_ptr<CaffEnt::Component> create() const; \
	};														\
	
// Helper Macros for defining component factories. Enter the name of component
// which will produce a factory with the same name and Factory appended at the end.
// ie. COMPONENT_FACTORY_HEADER(FooComponent) = FooComponentFactory
// Warning: breaking this convention will lead to issues as name is used to identify
// The component as well as create the name.
// This one is for the soruce of the factory
#define COMPONENT_FACTORY_SOURCE(name)									\
	std::unique_ptr<CaffEnt::Component> name##Factory::create() const { \
		return CaffEnt::ComponentUniquePtr(new name);					\
		}																\


namespace Caffeine {
namespace Systems {

struct EntityData
{
	time_t					xmlTimeStamp;
	time_t					modelTimeStamp;
	
	std::string				xmlFileName;
	std::string				modelFileName;
	
}; // struct

struct EntityComponentDetails
{
	std::string	name;
	std::string	params;
};


struct EntityDetails
{
	time_t			timeStamp;

	std::string		name;
	bool			isVisible;
	
	std::vector<EntityComponentDetails> components;
	std::vector<EntityDetails>			children;
};


class EntityFactory
{
public:

	explicit							EntityFactory();
										~EntityFactory();

	void								checkForUpdates();
				
	std::unique_ptr<CaffEnt::Entity>	createInstance(const std::string & id);
	
	bool								registerComponent(const std::string & factoryName, ComponentFactory *factory);
	void								clearFactories();
	
private:

	std::map<std::string, EntityDetails>			m_registeredEntities;
	std::map<std::string, EntityData>				m_registeredEnties;
	std::map<std::string, const ComponentFactory*>	m_registeredComponentFactories;

}; // class



# pragma mark - Factorys for system components.

class ComponentFactory
{
public:

	ComponentFactory() {}
	virtual ~ComponentFactory() {}
	
	virtual std::unique_ptr<Entity::Component> create() const = 0;

}; // class


class TransformComponentFactory : public ComponentFactory
{
public:
	explicit TransformComponentFactory() {}
	~TransformComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

class PhysicsComponentFactory : public ComponentFactory
{
public:
	explicit PhysicsComponentFactory() {}
	~PhysicsComponentFactory() {}

	std::unique_ptr<Entity::Component> create() const;
};

} // namespace
} // namespace

#endif // namespace
