
#include <Application/Components/CarrierModelComponent.hpp>

#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Utilities/XMLHelpers.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>

#include <BulletDynamics/btBulletDynamicsCommon.h>


namespace App {


CarrierModelComponent::CarrierModelComponent()
: CaffEnt::Component()
, m_spawnPoint(0.f, 0.f, 0.f)
{
}


CarrierModelComponent::~CarrierModelComponent()
{
}


void CarrierModelComponent::onStart()
{
	
}


void CarrierModelComponent::onEnd()
{
}


void CarrierModelComponent::loadParams(const std::string &params)
{
	tinyxml2::XMLDocument doc;
	doc.Parse(params.c_str());
	
	tinyxml2::XMLElement *root = doc.FirstChildElement("params");
	
	// Spawn point
	{
		std::string tokenStr;
		CaffUtil::XMLGetTokens(tokenStr, root, "param", "spawn_point");
		
		if(tokenStr != "")
		{
			std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(tokenStr);
			
			if(tokens.size() == 3)
			{
				m_spawnPoint = CaffUtil::MakeVec3(tokens.at(0), tokens.at(1), tokens.at(2));
			}
		}
	}
}


void CarrierModelComponent::onAttach()
{
	
}


void CarrierModelComponent::onDisattach()
{
}


void CarrierModelComponent::onCollision(const CaffEnt::ColliderDetails &collider)
{
}


void CarrierModelComponent::onThink(const float dt)
{
}


glm::vec3 CarrierModelComponent::getSpawnPoint() const
{
	// TODO: Clean up after transform is made better.
	CaffComp::TransformComponent *transform = getOwner()->getTransform();
	CaffComp::PhysicsComponent   *physics	= getOwner()->getRigidBody();
	
	const btVector3 fwd = physics->getRigidBody()->getWorldTransform().getBasis().getColumn(2);
	
	const glm::vec3 forward(fwd.x(), fwd.y(), fwd.z());
	const glm::vec3 result = transform->getPosition() + (m_spawnPoint - forward);
	
	return result;
}


glm::vec3 CarrierModelComponent::getSpawnRotation() const
{
	return glm::vec3(0.f, 0.f, 0.f);
}


COMPONENT_FACTORY_SOURCE(CarrierModelComponent)


} // namespace
