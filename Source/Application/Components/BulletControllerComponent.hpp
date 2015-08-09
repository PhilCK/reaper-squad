
#ifndef APPLICATION_BULLET_CONTROLLER_INCLUDED
#define APPLICATION_BULLET_CONTROLLER_INCLUDED

#include <Caffeine/Components/ControllerComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class BulletControllerComponent : public CaffComp::ControllerComponent
{
public:

	explicit			BulletControllerComponent();
						~BulletControllerComponent();
				
	void				onAttach() override;
	void				onDisattach() override;
	void				onThink(const float dt) override;
	
	void				eventController(const CaffSys::Event *eventData) override;

}; // class

COMPONENT_FACTORY_INTERFACE(BulletControllerComponent)

} // namespace

#endif // include guard
