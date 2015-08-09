
/*
	Controller Component
	--------------------
	
	Project:
	Caffeine
	
	Purpose:
	Controller component. Inherit from to make your controller.
	
	This was through to make MVC abit easier should I ever want to pipeline
	the objects, however events are going to be included at entity level so collisions
	and other stuff can be use the route.
 */

#ifndef CAFFEINE_COMPONENTS_CONTROLLER_INCLUDED
#define CAFFEINE_COMPONENTS_CONTROLLER_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/Event.hpp>

namespace Caffeine {
namespace Components {


class ControllerComponent : public Entity::Component, public CaffSys::Eventable
{
public:

	COMPONENT_RTTI(Controller)
	
	explicit				ControllerComponent();
							~ControllerComponent();

}; // class


} // namespace
} // namespace

#endif // include guard
