
#include <Application/Components/BulletModelComponent.hpp>

#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Entity/Entity.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace App {

BulletModelComponent::BulletModelComponent()
: CaffEnt::Component()
, m_startPos(0.f,0.f,0.f)
, m_endPos(0.f,0.f,0.f)
, m_currentPos(0.f,0.f,0.f)
, m_direction(0.f,0.f,0.f)
, m_color(0.f,0.f,0.f)
, m_speed(0.f)
, m_scatter(0.f)
, m_fired(false)
, counter(0)
, ray(nullptr)
{
}


BulletModelComponent::~BulletModelComponent()
{
}


void BulletModelComponent::onAttach()
{
}


void BulletModelComponent::onDisattach()
{
}


void BulletModelComponent::onThink(const float dt)
{
	// Move to controller.
	if(m_fired)
	{
		m_lastPosition = m_currentPos;
		m_currentPos  += (m_direction * dt) * 120.0f;
		
		// Check Ray
		{
			const glm::vec3 startPos = m_currentPos;
			const glm::vec3 endPos	 = m_lastPosition;
		
			const btVector3 start(startPos.x, startPos.y, startPos.z);
			const btVector3 end(endPos.x, endPos.y, endPos.z);
		
			btCollisionWorld::ClosestRayResultCallback rayd(start, end);
		
			CaffServ::PhysicsManager().getWorld().rayTest(start, end, rayd);
			
			if(rayd.hasHit())
			{
				const btVector3 endPoint = rayd.m_hitPointWorld;
				void * userPtr = rayd.m_collisionObject->getUserPointer();
				
				if(userPtr)
				{
					if(userPtr != (void*)getOwner())
					{
						getOwner()->setActive(false);
					
						CaffEnt::Entity *entity = static_cast<CaffEnt::Entity *>(userPtr);
						
						CaffEnt::ColliderDetails details;
						details.collidedWith = getOwner();
						
						entity->onCollision(details);
					}
				}
			}
		}
		
		
	}
	
	counter += dt;
	
	if(counter > 1.0)
	{
		getOwner()->setActive(false);
	}
}


void BulletModelComponent::setStart(const glm::vec3 &startPoint, const glm::vec3 &direction)
{
	m_startPos   = startPoint;
	m_currentPos = m_startPos;
	m_direction  = direction;
	m_endPos     = m_startPos + (m_direction * 5.f);
	
	m_fired = true;
}


COMPONENT_FACTORY_SOURCE(BulletModelComponent)

} // namespace
