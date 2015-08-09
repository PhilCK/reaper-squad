
#ifndef CAFFEINE_ENTITY_ENTITY_INCLUDED
#define CAFFEINE_ENTITY_ENTITY_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Components/ComponentsFwd.hpp>
#include <string>
#include <memory>
#include <vector>
#include <assert.h>

namespace Caffeine {
namespace Entity {


typedef Entity*						EntityPtr;
typedef std::unique_ptr<Entity>		EntityUniquePtr;

/*
	Entity
	------
	Entities form the game objects, they are not to be inheritied from.
	UniquePtrs are used extensivly here, an Entity will own its child entities.
	it will also own its components. You can remove child enties and components
	and you will get a UniquePtrs in return that you can attach to other entities.
	
	Entities contain some sugar methods to access common components, these will return 
	nullptr if they are not added to the entity. As the renderable component is inherited
	only the first attached component will be visible in the sugar.
*/

struct ColliderDetails
{
	EntityPtr collidedWith;
	// normal etc.
};

class Entity final
{
public:
    
    explicit						Entity(const std::string & name = "");
									~Entity();
	
		
    std::string						getName() const;
	void							setName(const std::string & setName);
	
	bool							isActive() const { return m_isActive; }
	void							setActive(const bool isActive);
	
	
	#pragma mark - Useage Hooks
	
	void							onStart();
	void							onEnd();
	
	// Access to the components and subentity update functions.
	void							onEarlyThink(const float dt);
	void							onThink(const float dt);
	void							onLateThink(const float dt);
	
	void							onCollision(const ColliderDetails &entity);
	
	// Access to the components render fuctions. TODO: This is in alot flux.
	void							onPreRender() const; // TODO: Unsure how this is goint to work atm.
	void							onRender() const;
	void							onPostRenderer() const; // TODO: Unsure how this is going to work atm.
	
	
	#pragma mark - Relationships
	
	// Returns the attached parent of a given entity, nullptr if none.
	EntityPtr						getParent() const;
	
	// Adds a child entity to the hierachy, it will take ownership of the object.
	void							addChild(EntityUniquePtr & setChild);
	
	// Removes a child from the hierachy, takes a string or a pointer. Returns a unique ptr
	// so it can be attached to another entity, or nullptr if none is found.
	EntityUniquePtr					removeChild(const std::string & childName);
	EntityUniquePtr					removeChild(const EntityPtr childPtr);
	
	// Will find the first instance of a child with the same name, nullptr if none.
	EntityPtr						findChild(const std::string & name) const;
	
	std::size_t						numberOfChildren() const;
	
	
	#pragma mark - Components
	
	// Adds a component. Only one component per type can be attached per entity.
	void							addComponent(ComponentUniquePtr & setComponent);
	void							addComponent(ComponentUniquePtr & setComponent, const std::string &params);
	
	// Removes a component, returns a unique ptr so it can be attached to another entity,
	// or nullptr if none is found.
	template<typename T>
	ComponentUniquePtr				removeComponent(const ComponentPtr componentPtr)
	{
		assert(componentPtr);
		
		// Remove child.
		auto it  = m_components.begin();
		auto end = m_components.end();
		
		for(; it != end; ++it)
		{
			if((*it).get() == componentPtr)
			{
				ComponentUniquePtr component = std::move(*it);
				m_components.erase(it);
				
				return component;
			}
		}
		
		return ComponentUniquePtr();

	}
	
	// Gets the component of requested type, returns nullptr if none is found.
	template<typename T>
	T*								getComponent() const
	{
		const std::string componentID = T::getTypeID();
		
		auto it  = m_components.begin();
		auto end = m_components.end();
		
		for(; it != end; ++it)
		{
			if((*it)->getInstTypeID() == componentID)
			{
				return static_cast<T*>((*it).get());
			}
		}
		
		return nullptr;
	}
	
	std::size_t						numberOfComponents() const;
	
	
	#pragma mark - Component Shortcuts
	
	// Returns nullptr if not transform has been attached.
	CaffComp::TransformComponent *	getTransform() const;
	
	// Returns nullptr if no rigidbody has been attached.
	CaffComp::PhysicsComponent *	getRigidBody() const;
	
	// As Renderable is inherited from you can attach more than one render class, in this case only the first
	// class attached to the entity will be used here. It will return nullptr if none has been attached.
	// TODO: This might have to die, why is this helpful? views should be automous, and rely on the model, and they are inherited from anyway.
	CaffComp::RenderableComponent *	getRenderer() const;
	
private:

	EntityPtr						m_parent;
	
    std::vector<EntityUniquePtr> 	m_children;
    std::vector<ComponentUniquePtr>	m_components;
	
	std::string						m_name;
	bool							m_isActive;
	
	CaffComp::TransformComponent*	m_transformComponent;
	CaffComp::PhysicsComponent*		m_rigidbodyComponent;
	CaffComp::RenderableComponent*	m_rendererComponent;
	
}; // class
    
} // namespace
} // namespace

#endif // include guard
