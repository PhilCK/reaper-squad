
#include <Application/Managers/EnemyFighterManagerComponent.hpp>

#include <Application/Common/ApplicationEvents.hpp>
#include <Application/Common/AppConstants.hpp>

#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Systems/EventManager.hpp>

namespace App {


EnemyFighterManagerComponent::EnemyFighterManagerComponent()
: CaffComp::ControllerComponent()
{
}


EnemyFighterManagerComponent::~EnemyFighterManagerComponent()
{
}


void EnemyFighterManagerComponent::onAttach()
{
	CaffServ::EventManager().addController(this, (unsigned int)EventID::ENEMY_FIGHTER_DAMAGED_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::ENEMY_FIGHTER_DESTROYED_MSG);
}


void EnemyFighterManagerComponent::onStart()
{
	CaffSys::Event evt = SetupAddNewEntity(AppConstant::entityEnemyFighter);
	CaffServ::EventManager().sendInstantEvent(&evt);
}


void EnemyFighterManagerComponent::onDisattach()
{
	CaffServ::EventManager().removeController(this, (unsigned int)EventID::ENEMY_FIGHTER_DAMAGED_MSG);
	CaffServ::EventManager().removeController(this, (unsigned int)EventID::ENEMY_FIGHTER_DESTROYED_MSG);
}


void EnemyFighterManagerComponent::eventController(const CaffSys::Event *eventData)
{
	switch(eventData->id)
	{
		case (unsigned int)EventID::ENEMY_FIGHTER_DAMAGED_MSG:
		break;
		
		case (unsigned int)EventID::ENEMY_FIGHTER_DESTROYED_MSG:
		{
			CaffSys::Event evt = SetupAddNewEntity(AppConstant::entityEnemyFighter);
			CaffServ::EventManager().sendInstantEvent(&evt);
			
			break;
		}
	}
}


COMPONENT_FACTORY_SOURCE(EnemyFighterManagerComponent)


} // namespace
