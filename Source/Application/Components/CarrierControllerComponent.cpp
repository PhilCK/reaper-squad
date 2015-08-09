
#include <Application/Components/CarrierControllerComponent.hpp>

#include <Application/Common/ApplicationEvents.hpp>
#include <Application/Common/AppConstants.hpp>
#include <Application/Components/CarrierModelComponent.hpp>

#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>

#include <BulletDynamics/btBulletDynamicsCommon.h>

namespace App {


CarrierControllerComponent::CarrierControllerComponent()
: CaffComp::ControllerComponent()
, m_hasPlayer(false)
, m_launchPlayer(false)
, m_maxTimer(1.f)
, m_controlTimer(0.f)
{
}


CarrierControllerComponent::~CarrierControllerComponent()
{
}


void CarrierControllerComponent::onStart()
{
	// Get the Carriers model.
	CarrierModelComponent *model = getOwner()->getComponent<CarrierModelComponent>();
	
	// Spawn the player.
	{
		CaffSys::Event evt = SetupAddNewEntity(AppConstant::entityPlayer);
		CaffServ::EventManager().sendInstantEvent(&evt);
		
		m_hasPlayer = true; // TODO: Is it tho? anyway to check?
		m_launchPlayer = false;
		m_controlTimer = 0.f;
	}
	
	// Send teleport message so that player is in correct position.
	{
		const glm::vec3 pos = model->getSpawnPoint();
		const glm::vec3 rot = model->getSpawnRotation();
		
		CaffSys::Event evt = SetupPlayerTeleport(pos, rot);
		CaffServ::EventManager().sendInstantEvent(&evt);
	}
	
	// Prevent user control
	{
		CaffSys::Event evt;
		evt.id = (unsigned int)EventID::INPUT_PLANE_SUSPENDED_MSG;
		CaffServ::EventManager().sendInstantEvent(&evt);
	}
}


void CarrierControllerComponent::onAttach()
{
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_ENTER_MSG);
}


void CarrierControllerComponent::onDisattach()
{
	CaffServ::EventManager().removeController(this, (unsigned int)EventID::INPUT_ENTER_MSG);
}


void CarrierControllerComponent::onThink(const float dt)
{
	if(m_hasPlayer && m_launchPlayer)
	{
		m_controlTimer += dt;
		
		if(m_controlTimer > m_maxTimer)
		{
			CaffSys::Event evt;
			evt.id = (unsigned int)EventID::INPUT_PLANE_RESUME_MSG;
			
			CaffServ::EventManager().sendInstantEvent(&evt);
			
			m_hasPlayer = false;
			m_launchPlayer = false;
		}
		else
		{
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_BOOST_MSG, 1.f);
			CaffServ::EventManager().sendInstantEvent(&evt);
		}
	}
	
}


void CarrierControllerComponent::eventController(const CaffSys::Event *eventData)
{
	if(IsAppEvent(eventData, EventID::INPUT_ENTER_MSG))
	{
		m_launchPlayer = true;
	}
}


COMPONENT_FACTORY_SOURCE(CarrierControllerComponent)

} // namespace
