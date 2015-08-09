/*
	PlayerControllerComponent.cpp
	-----------------------------
	
	Project: Reaper Squad
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#include <Application/Components/PlayerControllerComponent.hpp>
#include <Application/Components/ChaseCameraComponent.hpp>
#include <Application/Common/ApplicationEvents.hpp>
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Caffeine/Systems/CameraManager.hpp>
#include <Caffeine/Systems/Camera.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <Caffeine/Math/Math.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assert.h>

namespace App {

PlayerControllerComponent::PlayerControllerComponent()
: Caffeine::Entity::Component()
, Caffeine::Systems::Eventable()
, m_rotation(0)
, m_inputRot(0)
, m_playerModel(nullptr)
, m_active(true)
, m_coolDown(0.2f)
, m_currentCoolDown(3.f)
{
}



PlayerControllerComponent::~PlayerControllerComponent()
{
}


void PlayerControllerComponent::onStart()
{
	m_playerModel = getOwner()->getComponent<PlayerModelComponent>();
	assert(m_playerModel);
}



void PlayerControllerComponent::onEnd()
{
}


void PlayerControllerComponent::onAttach()
{
	CaffServ::EventManager().addController(this, (unsigned int)EventID::CAMERA_SELECT_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_PLANE_PITCH_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_PLANE_ROLL_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_PLANE_YAW_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_PLANE_SHOOT_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_PLANE_BOOST_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_PLANE_SLOW_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::PLAYER_TELEPORT_MSG);
	
	CaffComp::TransformComponent *transform = getOwner()->getTransform();
	transform->setRotation(glm::vec3(0, 0, 0));
	transform->setPosition(glm::vec3(0, 6, 0));
}


void PlayerControllerComponent::onDisattach()
{
	CaffServ::EventManager().removeController(this);
}


void PlayerControllerComponent::onEarlyThink(const float dt)
{
	m_currentCoolDown += dt;
}


void PlayerControllerComponent::onThink(const float dt)
{

}


void PlayerControllerComponent::onLateThink(const float dt)
{
}


void PlayerControllerComponent::eventController(const Caffeine::Systems::Event *eventData)
{
	if(m_active)
	{
		switch (eventData->id)
		{
			// Plane movement.
			case (unsigned int)EventID::INPUT_PLANE_PITCH_MSG:
			m_playerModel->applyElevator(eventData->left.fData);
			break;
			
			case (unsigned int)EventID::INPUT_PLANE_ROLL_MSG:
			m_playerModel->applyAileron(eventData->left.fData);
			break;
			
			case (unsigned int)EventID::INPUT_PLANE_YAW_MSG:
			m_playerModel->applyRudder(eventData->left.fData);
			break;
			
			case (unsigned int)EventID::INPUT_PLANE_SHOOT_MSG:
			{
				// Move to Model or GunComponent
				if(m_currentCoolDown > m_coolDown)
				{
					m_currentCoolDown = 0;
					
					const CaffComp::TransformComponent *transform = getOwner()->getTransform();
					const CaffComp::PhysicsComponent   *physics   = getOwner()->getRigidBody();
					
					const btVector3 phyFwd = physics->getRigidBody()->getWorldTransform().getBasis().getColumn(2);
					const btVector3 phyRight = physics->getRigidBody()->getWorldTransform().getBasis().getColumn(0);
					
					const glm::vec3 offset(phyRight.x(), phyRight.y(), phyRight.z());
					const glm::vec3 fwd(phyFwd.x(), phyFwd.y(), phyFwd.z());
					
					const float fwdOffset = 1.5f;
					
					// Wing mount 1
					{
						const glm::vec3 pos = transform->getPosition() - (offset * 1.0f);
						const glm::vec3 start = pos + (fwd * fwdOffset);
						
						const CaffSys::Event evt = SetupBullet(start, fwd);
						CaffServ::EventManager().sendInstantEvent(&evt);
					}
					
					// Wing mount 2
					{
						const glm::vec3 pos = transform->getPosition() + (offset * 1.0f);
						const glm::vec3 start = pos + (fwd * fwdOffset);
					
						const CaffSys::Event evt = SetupBullet(start, fwd);
						CaffServ::EventManager().sendInstantEvent(&evt);
					}
					
					// Wing mount 3
					{
						const glm::vec3 pos = transform->getPosition() - (offset * 2.0f);
						const glm::vec3 start = pos + (fwd * fwdOffset);
						
						const CaffSys::Event evt = SetupBullet(start, fwd);
						CaffServ::EventManager().sendInstantEvent(&evt);
					}
					
					// Wing mount 4
					{
						const glm::vec3 pos = transform->getPosition() + (offset * 2.0f);
						const glm::vec3 start = pos + (fwd * fwdOffset);
					
						const CaffSys::Event evt = SetupBullet(start, fwd);
						CaffServ::EventManager().sendInstantEvent(&evt);
					}
				}
			}
			break;
			
			case (unsigned int)EventID::INPUT_PLANE_BOOST_MSG:
			m_playerModel->applyThrust(+1.0f);
			break;

			case (unsigned int)EventID::INPUT_PLANE_SLOW_MSG:
			m_playerModel->applyThrust(-1.0f);
			break;
			
			case (unsigned int)EventID::PLAYER_TELEPORT_MSG:
			{
				//CaffComp::TransformComponent *transform = getOwner()->getTransform();
				CaffComp::PhysicsComponent *physics = getOwner()->getRigidBody();
				
				//const glm::vec3 pos();
				
				// TODO: Add rotation when transform rewrite.
				physics->getRigidBody()->getWorldTransform().setOrigin(btVector3(eventData->left.vData.at(0), eventData->left.vData.at(1), eventData->left.vData.at(2)));
				//transform->setPosition(pos);
			}
			break;
		}
	}
	
	switch(eventData->id)
	{
		case (unsigned int)EventID::CAMERA_SELECT_MSG:
		{
			if(!strcmp(eventData->left.sData, "ChaseCam"))
			{
				ChaseCameraComponent *cam = getOwner()->getComponent<ChaseCameraComponent>();
				cam->makeActive();
				m_active = true;
			}
			else
			{
				m_active = false;
			}
		}
		break;
	}
}

COMPONENT_FACTORY_SOURCE(PlayerControllerComponent)

} // include guard
