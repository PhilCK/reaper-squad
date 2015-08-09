
#ifndef CAFFEINE_ENTITY_SIMPLE_ENTITY_INCLUDED
#define CAFFEINE_ENTITY_SIMPLE_ENTITY_INCLUDED


#include <Caffeine/Entity/EntityFwd.hpp>
#include <map>
#include <string>

namespace Caffeine {
namespace Entity {

/*
	SimpleEntity
	------------
	A stop gap messure until a full entity system can be made.
	This provides a system of adding components, but not of adding
	sub entities.
	
	TODO: Using strings at identifiers isn't very good really, would prefer getComp<YourComp>();

 */
class SimpleEntity final
{
public:

	explicit					SimpleEntity();
								~SimpleEntity();
	
	#pragma mark - Components
	
	void						addComponent(const std::string name, Component *newComp);
	Component*					getComponent(const std::string name);
	std::size_t					getNumberOfComponents() const;
	
	
	#pragma mark - Thinking
	
	// These are hooks to call the updates on the components.
	void						onEarlyThink(const float dt);
	void						onThink(const float dt);
	void						onLateThink(const float dt);

	
private:

	std::map<std::string, Component*> m_components;

}; // class


} // namepace
} // namespace

#endif // namespace
