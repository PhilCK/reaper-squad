
#ifndef CAFFEINE_ENTITY_COMPONENT_INCLUDED
#define CAFFEINE_ENTITY_COMPONENT_INCLUDED

#include <Caffeine/Platform.hpp>
#include <Caffeine/Entity/EntityFwd.hpp>
#include <Caffeine/Systems/SystemFwd.hpp>
#include <string>

namespace Caffeine {
namespace Entity {

typedef Component*					ComponentPtr;
typedef std::unique_ptr<Component> 	ComponentUniquePtr;

#define COMPONENT_RTTI(name)									\
		static std::string getTypeID()	{ return #name; }		\
		std::string getInstTypeID()		{ return #name; }		\


class Component
{
	friend class SimpleEntity;
	friend class Entity;

public:

	explicit				Component();
	virtual					~Component() {}
	
	virtual	std::string		getInstTypeID() { return "none"; }
	
	virtual void			loadParams(const std::string &params) {} //TODO: private?
	
	inline Entity*			getOwner() const			{ return m_owner; }
	inline void				setOwner(Entity *entity)	{ m_owner = entity; }
	
// Grumble needs to be public for hook method in entity. private:

	virtual void			onStart() {}
	virtual void			onEnd() {}
	
	virtual	void			onAttach() {}
	virtual void			onDisattach() {}
	
	virtual void			onEarlyThink(const float dt) {}
	virtual void			onThink(const float dt) {}
	virtual void			onLateThink(const float dt) {}
	
	//virtual void			onCollision(const CaffEnt::Entity *entity) {}
	virtual void			onCollision(const ColliderDetails &collider) {}
	
	//virtual void			onEvent(const Systems::Event * data) {}

private:
	
	Entity*						m_owner;
	//Systems::Eventable		m_eventable;
	
}; // class


} // namespace
} // namespace

#endif
