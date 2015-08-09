
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Entity/Component.hpp>

#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Components/RenderableComponent.hpp>

namespace
{
	// Updates hooks that take no paramaters.
	template<typename S, typename T>
	void GenericOnHook(S &container, void (T::*hook)(void))
	{
		auto it  = container.begin();
		auto end = container.end();
		
		for(; it != end; ++it)
		{
			((*it).get()->*hook)();
		}
	}

	// Updates hooks that take deltatime
	template<typename S, typename T>
	void OnThinkHook(S &container, void (T::*hook)(const float), const float dt)
	{
		auto it  = container.begin();
		auto end = container.end();
		
		for(; it != end; ++it)
		{
			((*it).get()->*hook)(dt);
		}
	}

	// Updates hooks that take deltatime
	template<typename S, typename T>
	void OnCollisionHook(S &container, void (T::*hook)(const CaffEnt::ColliderDetails &), const CaffEnt::ColliderDetails &collider)
	{
		auto it  = container.begin();
		auto end = container.end();
		
		for(; it != end; ++it)
		{
			((*it).get()->*hook)(collider);
		}
	}
}


namespace Caffeine {
namespace Entity {

// Size hints for the components and children.
#define CHILD_STD_VEC_HINT 4
#define COMP_STD_VEC_HINT  8

Entity::Entity(const std::string & name)
: m_parent(nullptr)
, m_children()
, m_components()
, m_name(name)
, m_isActive(true)
, m_transformComponent(nullptr)
, m_rigidbodyComponent(nullptr)
, m_rendererComponent(nullptr)
{
	m_children.reserve(CHILD_STD_VEC_HINT);
	m_components.reserve(COMP_STD_VEC_HINT);
}



Entity::~Entity()
{
}



std::string Entity::getName() const
{
	if(!m_name.empty()) {
		return m_name;
	}
	else {
		return m_parent ? "Sub-Entity" : "Entity";
	}
}



void Entity::setName(const std::string & setName)
{
	m_name = setName;
}


void Entity::setActive(const bool isActive)
{
	m_isActive = isActive;
}


#pragma mark - Useage Hooks

void Entity::onStart()
{
	GenericOnHook(m_components, &Component::onStart);
	GenericOnHook(m_children,   &Entity::onStart);
}





void Entity::onEnd()
{
	GenericOnHook(m_components, &Component::onEnd);
	GenericOnHook(m_children,   &Entity::onEnd);
}



void Entity::onEarlyThink(const float dt)
{
	OnThinkHook(m_components, &Component::onEarlyThink, dt);
	OnThinkHook(m_children,   &Entity::onEarlyThink,    dt);
}



void Entity::onThink(const float dt)
{
	OnThinkHook(m_components, &Component::onThink, dt);
	OnThinkHook(m_children,   &Entity::onThink,    dt);
}



void Entity::onLateThink(const float dt)
{
	OnThinkHook(m_components, &Component::onLateThink, dt);
	OnThinkHook(m_children,   &Entity::onLateThink,	   dt);
}


void Entity::onCollision(const ColliderDetails &collider)
{
	OnCollisionHook(m_components, &Component::onCollision, collider);
	OnCollisionHook(m_children, &Entity::onCollision, collider);
}


void Entity::onPreRender() const
{
}



void Entity::onRender() const
{
}



void Entity::onPostRenderer() const
{
}



#pragma mark - Relationships

EntityPtr Entity::getParent() const {
	return m_parent;
}



void Entity::addChild(EntityUniquePtr & setChild)
{
	setChild->m_parent = this;
	m_children.push_back(std::move(setChild));
}



EntityUniquePtr Entity::removeChild(const std::string &childName)
{
	return removeChild(findChild(childName));
}



EntityUniquePtr Entity::removeChild(const EntityPtr childPtr)
{
	assert(childPtr);
	
	// Remove child.
	auto it  = m_children.begin();
	auto end = m_children.end();
	
	for(; it != end; ++it)
	{
		if((*it).get() == childPtr)
		{
			EntityUniquePtr child = std::move(*it);
			m_children.erase(it);
			
			return child;
		}
	}
	
	return EntityUniquePtr();
}



EntityPtr Entity::findChild(const std::string &name) const
{
	auto it  = m_children.begin();
	auto end = m_children.end();
	
	for(; it != end; ++it)
	{
		if((*it)->getName() == name)
		{
			return (*it).get();
		}
	}

	return nullptr;
}



std::size_t Entity::numberOfChildren() const
{
	return m_children.size();
}



#pragma mark - Components

void Entity::addComponent(ComponentUniquePtr &setComponent, const std::string &params)
{
	m_components.push_back(std::move(setComponent));
	m_components.back()->setOwner(this);
	m_components.back()->loadParams(params);
	m_components.back()->onAttach();
}

void Entity::addComponent(ComponentUniquePtr & setComponent)
{
	m_components.push_back(std::move(setComponent));
	m_components.back()->setOwner(this);
	m_components.back()->onAttach();
}
	
	
	
std::size_t Entity::numberOfComponents() const
{
	return m_components.size();
}



#pragma mark - Component Shortcuts

CaffComp::TransformComponent* Entity::getTransform() const	{ return getComponent<CaffComp::TransformComponent>(); }
CaffComp::PhysicsComponent* Entity::getRigidBody() const	{ return getComponent<CaffComp::PhysicsComponent>(); }
CaffComp::RenderableComponent* Entity::getRenderer() const	{ return getComponent<CaffComp::RenderableComponent>(); }

   
} // namespace
} // namespace
