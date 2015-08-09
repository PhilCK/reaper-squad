
#include <Application/Managers/BulletManagerComponent.hpp>

#include <Application/Common/ApplicationEvents.hpp>
#include <Application/Components/BulletModelComponent.hpp>

#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/EventManager.hpp>

namespace App {


BulletManagerComponent::BulletManagerComponent()
: CaffComp::ControllerComponent()
{
}


BulletManagerComponent::~BulletManagerComponent()
{
}


void BulletManagerComponent::onAttach()
{
	CaffServ::EventManager().addController(this, (unsigned int)App::EventID::PLANE_SHOOT_MSG);
}


void BulletManagerComponent::onDisattach()
{
	CaffServ::EventManager().removeController(this, (unsigned int)App::EventID::PLANE_SHOOT_MSG);
}


void BulletManagerComponent::eventController(const CaffSys::Event *eventData)
{
	if(eventData->id == (unsigned int)App::EventID::PLANE_SHOOT_MSG)
	{
		CaffSys::EntityFactory &entityFactory = CaffServ::EntityFactory();
		
		glm::vec3 startV, dirV;
		
		startV.x = eventData->left.vData[0];
		startV.y = eventData->left.vData[1];
		startV.z = eventData->left.vData[2];
		
		dirV.x = eventData->right.vData[0];
		dirV.y = eventData->right.vData[1];
		dirV.z = eventData->right.vData[2];
	
		CaffEnt::EntityUniquePtr bullet = entityFactory.createInstance("Bullet");
		bullet->getComponent<BulletModelComponent>()->setStart(startV, dirV);
		
		AddEntityEventData addEntity(std::move(bullet));
		
		CaffServ::EventManager().sendInstantEvent(&addEntity);
	}
}


COMPONENT_FACTORY_SOURCE(BulletManagerComponent)


} // namespace
