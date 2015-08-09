
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Application/Common/ApplicationEvents.hpp>
#include <Caffeine/Components/TransformComponent.hpp>
#include <Application/Components/PlayerModelComponent.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Math/Math.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <LinearMath/btAabbUtil2.h>
#include <BulletCollision/btBulletCollisionCommon.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <Application/Components/ChaseCameraComponent.hpp>
#include <glm/glm.hpp>
#include <iostream>

namespace App {

PlayerModelComponent::PlayerModelComponent()
: CaffEnt::Component()
// Thrust
, m_thrust(0.0f)
, m_thrustAccel(7000.0f)
, m_maxThrust(100000.0f)
, m_minThrust(0.0f)
, m_applyThust(0.0f)
, m_thrustFalloff(0.8f)
// Rudder
, m_rudder(0.0f)
, m_rudderAccel(1000.0f)
, m_maxRudder(1000.0f)
, m_minRudder(-1000.0f)
, m_applyRudder(0.0f)
, m_rudderFalloff(0.5f)
// Elevator
, m_elevator(0.0f)
, m_elevatorAccel(1000.0f)
, m_maxElevator(1000.0f)
, m_minElevator(-1000.0f)
, m_applyElevator(0.0f)
, m_elevatorFalloff(0.5f)
// Airleron
, m_aileron(0.0f)
, m_aileronAccel(200.0f)
, m_maxAileron(200.0f)
, m_minAileron(-200.0f)
, m_applyAileron(0.0f)
, m_aileronFalloff(0.6f)
// Some flight dynamics
, m_lift(0.0f)
, m_maxLift(-10.0f)
{
}

	

PlayerModelComponent::~PlayerModelComponent()
{
}



void PlayerModelComponent::onStart()
{
	const glm::vec3 offset(0.f, -1.f, 4.f);
	const float elastic(0.5f);
	const float length(10.0f);
	const float maxFOV(85);
	const float minFOV(40);

	ChaseCameraComponent *cam = getOwner()->getComponent<ChaseCameraComponent>();
	cam->setCameraSettings(&offset[0], elastic, length, maxFOV, minFOV);
	cam->makeActive();
}



void PlayerModelComponent::onEnd()
{
}


void PlayerModelComponent::onAttach()
{
	// -- Phys
	
	CaffComp::PhysicsComponent *phys = getOwner()->getRigidBody();
		
	phys->setMass(9);

	const CaffSys::ModelData &collisonMesh = CaffServ::ModelManager().getModelData("SpitfireCollision");
	CaffApp::Mesh &mesh = collisonMesh.model->getMesh(0);
	
	btTriangleMesh *bulletMesh = new btTriangleMesh();

	// TODO: EEEEK I had to go into blender to get the number of tri's
	for(uint16_t i = 0; i < 26; ++i)
	{
		CaffApp::VertexContainer verts = mesh.getGLVertexBuffer();
		
		uint16_t vertA = (i * (8 * 3)) + (8 * 0);
		btVector3 a(verts[vertA + 0], verts[vertA + 1], verts[vertA + 2]);

		uint16_t vertB = (i * (8 * 3)) + (8 * 1);
		btVector3 b(verts[vertB + 0], verts[vertB + 1], verts[vertB + 2]);
		
		uint16_t vertC = (i * (8 * 3)) + (8 * 2);
		btVector3 c(verts[vertC + 0], verts[vertC + 1], verts[vertC + 2]);
		
		bulletMesh->addTriangle(a, b, c, false);
	}
	
	btConvexTriangleMeshShape* shape = new btConvexTriangleMeshShape(bulletMesh);
	
	//phys->setCollisionShape(shape);
	//std::unique_ptr<CaffComp::Collider> collider(new CaffComp::BoxCollider(1.f, 1.f, 2.f));
	//phys->setColliderShape(std::move(collider));
}


void PlayerModelComponent::onDisattach()
{
}


void PlayerModelComponent::onCollision(const CaffEnt::ColliderDetails &collider)
{
	
}


void PlayerModelComponent::onThink(const float dt)
{
	// Apply thrust!
	{
		m_applyThust *= dt;
		m_thrust *= (m_thrustFalloff * dt);
		
		m_thrust += (m_applyThust * m_thrustAccel);
		m_thrust  = CaffMath::Clamp(m_thrust, m_minThrust, m_maxThrust);
		
		m_applyThust = 0.0f;
	}
	
	// Elevator
	{
		m_applyElevator *= dt;
		m_elevator *= (m_elevatorFalloff * dt);
		
		m_elevator += (m_applyElevator * m_elevatorAccel);
		m_elevator  = CaffMath::Clamp(m_elevator, m_minElevator, m_maxElevator);
		
		m_applyElevator = 0.0f;
	}
	
	// Rudder
	{
		m_applyRudder *= dt;
		m_rudder *= (m_rudderFalloff * dt);
		
		m_rudder += (m_applyRudder * m_rudderAccel);
		m_rudder  = CaffMath::Clamp(m_rudder, m_minRudder, m_maxRudder);
		
		m_applyRudder = 0.0f;
	}
	
	// Aileron
	{
		m_applyAileron *= dt;
		m_aileron *= (m_aileronFalloff * dt);
		
		m_aileron += (m_applyAileron * m_aileronAccel);
		m_aileron  = CaffMath::Clamp(m_aileron, m_minAileron, m_maxAileron);
		
		m_applyAileron = 0.0f;
	}
	
	//
	
	
	//std::cout << m_thrust << std::endl;
	
	CaffComp::PhysicsComponent *rigidBody = getOwner()->getRigidBody();
	
	glm::vec3 force(0, 0, m_thrust * 2);
	rigidBody->applyLocalForce(&force[0]);
	
	glm::vec3 tail(m_elevator * 2, m_rudder * 2, m_aileron * 2);
	rigidBody->applyTorque(&tail[0]);
	
	//glm::vec3 lift(0,85,0);
	//rigidBody->applyForce(&lift[0]);
	
	CaffComp::TransformComponent *transform = getOwner()->getTransform();
	
	glm::vec3 pos = transform->getPosition();
	glm::vec3 dir = transform->getForwardVec();
	glm::vec3 upVec  = transform->getUpVec();
	
	btVector3 fwd = rigidBody->getRigidBody()->getWorldTransform().getBasis().getColumn(2);
	btVector3 up  = rigidBody->getRigidBody()->getWorldTransform().getBasis().getColumn(1);
	
	dir.x = fwd.x();
	dir.y = fwd.y();
	dir.z = fwd.z();
	
	upVec.x = up.x();
	upVec.y = up.y();
	upVec.z = up.z();
	
	ChaseCameraComponent *cam = getOwner()->getComponent<ChaseCameraComponent>();
	cam->setTargetPosition(&transform->getPosition()[0]);
	cam->setDirection(&upVec[0], &dir[0]);
}



void PlayerModelComponent::applyThrust(const float thrust)
{
	m_applyThust = thrust;
}



void PlayerModelComponent::applyAileron(const float aileron)
{
	m_applyAileron = aileron;
}



void PlayerModelComponent::applyElevator(const float elevator)
{
	m_applyElevator = elevator;
}



void PlayerModelComponent::applyRudder(const float rudder)
{
	m_applyRudder = rudder;
}


COMPONENT_FACTORY_SOURCE(PlayerModelComponent)

} // namespace
