
#ifndef APPLICATION_BULLET_MANAGER_COMPONENT_INCLUDED
#define APPLICATION_BULLET_MANAGER_COMPONENT_INCLUDED

#include <Caffeine/Components/ControllerComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class BulletManagerComponent : public CaffComp::ControllerComponent
{
public:

	COMPONENT_RTTI(BulletManager)

	explicit			BulletManagerComponent();
						~BulletManagerComponent();
				
	void				onAttach() override;
	void				onDisattach() override;
	
	void				eventController(const CaffSys::Event* eventData) override;

}; // class

COMPONENT_FACTORY_INTERFACE(BulletManagerComponent)
  
} // namespace

#endif // include guard
