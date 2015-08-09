
#include <Caffeine/Entity/SimpleEntity.hpp>
#include <Caffeine/Entity/Component.hpp>

namespace Caffeine {
namespace Entity {

SimpleEntity::SimpleEntity()
{
}


SimpleEntity::~SimpleEntity()
{
	// Delete all the components.
	for(auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		it->second->onDisattach();
		delete it->second;
	}
	
	m_components.clear();
}


#pragma mark - Components

void SimpleEntity::addComponent(const std::string name, Component *newComp)
{
	//newComp->setOwner(this);
	m_components.insert(std::pair<std::string, Component*>(name, newComp));
	newComp->onAttach();
}


Component* SimpleEntity::getComponent(const std::string name)
{
	return m_components[name];
}


std::size_t SimpleEntity::getNumberOfComponents() const
{
	return m_components.size();
}


#pragma mark - Thinking

void SimpleEntity::onEarlyThink(const float dt)
{
	for(auto comp : m_components)
	{
		comp.second->onEarlyThink(dt);
	}
}


void SimpleEntity::onThink(const float dt)
{
	for(auto comp : m_components)
	{
		comp.second->onThink(dt);
	}
}


void SimpleEntity::onLateThink(const float dt)
{
	for(auto comp : m_components)
	{
		comp.second->onLateThink(dt);
	}
}

} // namespace
} // namespace
