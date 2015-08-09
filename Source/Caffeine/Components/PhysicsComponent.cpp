
#include <Caffeine/Components/PhysicsComponent.hpp>

#include <Caffeine/Components/TransformComponent.hpp>
#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/PhysicsManager.hpp>
#include <Caffeine/Utilities/XMLHelpers.hpp>
#include <Caffeine/Utilities/StringHelpers.hpp>

#include <LinearMath/btAabbUtil2.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace {
	
	// Param primitive types.
	const std::string boxType		= "box";
	const std::string cubeType		= "cube";
	const std::string staticPlane	= "splane";


	// TODO: Support rotations in the xml.
	void ParamColliders(CaffComp::PhysicsComponent * component, std::vector<std::string> &tokens)
	{
		struct Details
		{
			std::string			type;
			std::vector<float>	numbers;
		};
		
		std::vector<Details> colliders;
		
		// First pull out all the types of colliders and information.
		for(auto it = tokens.begin(); it != tokens.end(); ++it)
		{
			if((*it) == boxType)
			{
				Details details;
				details.type = boxType;
				
				colliders.push_back(details);
			}
			else if((*it) == cubeType)
			{
				Details details;
				details.type = cubeType;
				
				colliders.push_back(details);
			}
			else if((*it) == staticPlane)
			{
				Details details;
				details.type = staticPlane;
				
				colliders.push_back(details);
			}
			else
			{
				if(!colliders.empty())
				{
					colliders.back().numbers.push_back(::atof((*it).c_str()));
				}
			}
		}
		
		// We use compound collider for everything just for ease sakes.
		CaffComp::CompoundCollider* compoundCollider(new CaffComp::CompoundCollider);
		
		
		// Produce the colliders.
		for(auto it = colliders.begin(); it != colliders.end(); ++it)
		{
			if((*it).type == boxType)
			{
				if(((*it).numbers.size() == 3) || ((*it).numbers.size() == 6))
				{
					const glm::vec3 extents((*it).numbers.at(0), (*it).numbers.at(1), (*it).numbers.at(2));
					
					glm::vec3 offsetPos(0.f, 0.f, 0.f);
					glm::vec3 offsetRot(0.f, 0.f, 0.f);
					
					if((*it).numbers.size() == 6)
					{
						offsetPos = glm::vec3((*it).numbers.at(3), (*it).numbers.at(4), (*it).numbers.at(5));
					}
					
					std::unique_ptr<CaffComp::BoxCollider> box(new CaffComp::BoxCollider(extents.x, extents.y, extents.z));
					
					compoundCollider->addCollider(offsetPos, offsetRot, std::move(box));
				}
			}
			else if((*it).type == cubeType)
			{
				if(((*it).numbers.size() == 1) || ((*it).numbers.size() == 4))
				{
					const float extents((*it).numbers.at(0));
					
					glm::vec3 offsetPos(0.f, 0.f, 0.f);
					glm::vec3 offsetRot(0.f, 0.f, 0.f);
					
					if((*it).numbers.size() == 4)
					{
						offsetPos = glm::vec3((*it).numbers.at(1), (*it).numbers.at(2), (*it).numbers.at(3));
					}
					
					std::unique_ptr<CaffComp::CubeCollider> cube(new CaffComp::CubeCollider(extents));
					
					compoundCollider->addCollider(offsetPos, offsetRot, std::move(cube));
				}
			}
			else if((*it).type == staticPlane)
			{
				if(((*it).numbers.size() == 4) || ((*it).numbers.size() == 7))
				{
					const glm::vec3 normal((*it).numbers.at(0), (*it).numbers.at(1), (*it).numbers.at(2));
					const float normalOffset = (*it).numbers.at(3);
					
					glm::vec3 offsetPos(0.f, 0.f, 0.f);
					glm::vec3 offsetRot(0.f, 0.f, 0.f);
					
					if((*it).numbers.size() == 7)
					{
						offsetPos = glm::vec3((*it).numbers.at(4), (*it).numbers.at(5), (*it).numbers.at(6));
					}
					
					std::unique_ptr<CaffComp::StaticPlaneCollider> splane(new CaffComp::StaticPlaneCollider(normal.x, normal.y, normal.z, normalOffset));
					
					compoundCollider->addCollider(offsetPos, offsetRot, std::move(splane));
				}
			}
		} // for loop
	
		std::unique_ptr<CaffComp::Collider> collider(compoundCollider);
		
		component->setColliderShape(std::move(collider));
		
	} // ParamColliders(...)

} // namespace

namespace Caffeine {
namespace Components {

#pragma mark - Colliders

Collider::Collider()
: m_collisionShape(nullptr)
{}


BoxCollider::BoxCollider(const float xExtends, const float yExtneds, const float zExtends)
: Collider()
, m_extends()
{
	m_extends.at(0) = xExtends;
	m_extends.at(1) = yExtneds;
	m_extends.at(2) = zExtends;
}


btCollisionShape * BoxCollider::getCollider()
{
	if(!m_collisionShape)
	{
		const btVector3 size(m_extends.at(0), m_extends.at(1), m_extends.at(2));
		m_collisionShape.reset(new btBoxShape(size));
	}
	
	return m_collisionShape.get();
}


CubeCollider::CubeCollider(const float extends)
: BoxCollider(extends, extends, extends)
{
}


StaticPlaneCollider::StaticPlaneCollider(const float xNormal, const float yNormal, const float zNormal, const float offset)
: Collider()
, m_normal()
, m_offset(offset)
{
	m_normal.at(0) = xNormal;
	m_normal.at(1) = yNormal;
	m_normal.at(2) = zNormal;
}


btCollisionShape * StaticPlaneCollider::getCollider()
{
	if(!m_collisionShape)
	{
		const btVector3 normal(m_normal.at(0), m_normal.at(1), m_normal.at(2));
		m_collisionShape.reset(new btStaticPlaneShape(normal, m_offset));
	}
	
	return m_collisionShape.get();
}


#define SHAPE_STD_VEC_HINT 4

CompoundCollider::CompoundCollider()
: Collider()
{
	m_shapes.reserve(SHAPE_STD_VEC_HINT);
}


void CompoundCollider::addCollider(const glm::vec3 &offsetPos, const glm::vec3 &offsetRot, std::unique_ptr<Collider> collider)
{
	std::unique_ptr<ColliderDetails> details(new ColliderDetails);
	
	details->pos	  = offsetPos;
	details->rot	  = offsetRot;
	details->collider = std::move(collider);
	
	m_shapes.push_back(std::move(details));
}


btCollisionShape * CompoundCollider::getCollider()
{
	if(!m_collisionShape)
	{
		btCompoundShape *compoundShape = new btCompoundShape();
		
		for(auto it = m_shapes.begin(); it != m_shapes.end(); ++it)
		{
			btCollisionShape *shape = (*it)->collider->getCollider();
			
			const btQuaternion	rotation((*it)->rot.x, (*it)->rot.y, (*it)->rot.z);
			const btVector3		position((*it)->pos.x, (*it)->pos.y, (*it)->pos.z);
			const btTransform	trans(rotation, position);
			
			compoundShape->addChildShape(trans, shape);
		}
		
		m_collisionShape.reset(compoundShape);
	}
	
	return m_collisionShape.get();
}


#pragma mark - Physics Components

PhysicsComponent::PhysicsComponent()
: Entity::Component()
, m_isTrigger(false)
, m_mass(0.f)
, m_rotAxis(X_AXIS | Y_AXIS | Z_AXIS)
, m_posAxis(X_AXIS | Y_AXIS | Z_AXIS)
, m_collisionShape(nullptr)
, m_motionState(nullptr)
, m_rigidbody(nullptr)
, m_rbodyInertia(0, 0, 0)
{
}


PhysicsComponent::~PhysicsComponent()
{
}


void PhysicsComponent::loadParams(const std::string &params)
{
	tinyxml2::XMLDocument doc;
	doc.Parse(params.c_str());
	
	tinyxml2::XMLElement *root = doc.FirstChildElement("params");
	
	// Collider Shapes
	{
		std::string tokenStr;
		CaffUtil::XMLGetTokens(tokenStr, root, "param", "collider");
		
		if(tokenStr != "")
		{
			std::vector<std::string> tokens = CaffUtil::SplitStringBySpaces(tokenStr);
			ParamColliders(this, tokens);
		}
	}
	
}


void PhysicsComponent::onEnd()
{
	if(m_rigidbody.get())
	{
		CaffServ::PhysicsManager().removeRigidbody(m_rigidbody.get());
	}
}


void PhysicsComponent::onEarlyThink(const float dt)
{
	// Update the transform
	// TODO: Make this happen before onEarlyThink().
	if(m_rigidbody)
	{
		CaffComp::TransformComponent *transform = getOwner()->getTransform();
		assert(transform);
		
		// We need the array aligned correctly for bullet physics.
		std::array<float, 16> mat __attribute__ ((aligned (16)));
		mat.at(15) = 1;
		
		btTransform trans;
		m_rigidbody->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(&mat[0]);
		
		const glm::mat4 phyMat = glm::make_mat4(&mat[0]);

		transform->setWorldMatrix(phyMat);
	}
}


void PhysicsComponent::setColliderShape(std::unique_ptr<Collider> collider)
{
	m_collisionShape = std::move(collider);
	updateRigidbody();
}


void PhysicsComponent::setMass(const float mass)
{
	m_mass = mass;
	updateRigidbody();
}


void PhysicsComponent::setPositionAxis(const uint8_t axis)
{
	m_posAxis = axis;
	updateRigidbody();
}


void PhysicsComponent::setRotationAxis(const uint8_t axis)
{
	m_rotAxis = axis;
	updateRigidbody();
}


void PhysicsComponent::setTrigger(const bool isTrigger)
{
	m_isTrigger = isTrigger;
	updateRigidbody();
}


void PhysicsComponent::applyForce(const float *dir)
{
	m_rigidbody->activate(true);
	m_rigidbody->applyCentralForce(btVector3(dir[0], dir[1], dir[2]));
	
	//m_pImpl->rigidBody->applyTorque(btVector3(0, 0, 1000));
	//m_pImpl->rigidBody->applyTorque(btVector3(0, 0, 1));
	//m_pImpl->rigidBody->setAngularVelocity(btVector3(0, 50, 0));
}


void PhysicsComponent::applyLocalForce(const float *dir, const float *offset)
{
	btVector3 force(dir[0], dir[1], dir[2]);
	btVector3 wForce = m_rigidbody->getWorldTransform().getBasis() * force;
	
	btVector3 relOffset(offset[0], offset[1], offset[2]);
	btVector3 wOffset = m_rigidbody->getWorldTransform().getOrigin() + relOffset;
	
	m_rigidbody->activate(true);
	m_rigidbody->applyForce(wForce, wOffset);
}


void PhysicsComponent::applyLocalForce(const float *dir)
{
	btVector3 force(dir[0], dir[1], dir[2]);
	btVector3 wForce = m_rigidbody->getWorldTransform().getBasis() * force;
	
	m_rigidbody->activate(true);
	m_rigidbody->applyCentralForce(wForce);
}


void PhysicsComponent::applyTorque(const float *dir)
{
	m_rigidbody->activate(true);
	
	btVector3 torque(dir[0], dir[1], dir[2]);
	btVector3 wTorque = m_rigidbody->getWorldTransform().getBasis() * torque;
	
	//m_pImpl->rigidBody->applyTorque(btVector3(dir[0], dir[1], dir[2]));
	m_rigidbody->applyTorque(wTorque);
}


void PhysicsComponent::updateRigidbody()
{
	if(m_rigidbody)
	{
		CaffServ::PhysicsManager().removeRigidbody(m_rigidbody.get());
	}
	
	if(m_collisionShape)
	{
		const CaffComp::TransformComponent *transform = getOwner()->getTransform();
		assert(transform);
		
		// Setup and Create rigidbody
		{
			const glm::vec3 pos = transform->getPosition();
			
			const btVector3		rPos(pos.x, pos.y, pos.z);
			const btQuaternion	rRot(0.f, 0.f, 0.f, 1.f);
			const btScalar		rMass(m_mass);
			
			m_motionState.reset(new btDefaultMotionState(btTransform(rRot, rPos)));
			
			btCollisionShape * shape = m_collisionShape->getCollider();
			
			shape->calculateLocalInertia(rMass, m_rbodyInertia);
			
			const btRigidBody::btRigidBodyConstructionInfo rbodyCI(rMass, m_motionState.get(), shape, m_rbodyInertia);
			
			m_rigidbody.reset(new btRigidBody(rbodyCI));
			
			m_rigidbody->setDamping(0.75f, 0.75f);
			m_rigidbody->setUserPointer((void*)getOwner());
		}
				
		// Constrain axis.
		{
			const btVector3 axisMovement((m_posAxis >> 0 & 1), (m_posAxis >> 1 & 1), (m_posAxis >> 2 & 1));
			const btVector3 axisRotation((m_rotAxis >> 0 & 1), (m_rotAxis >> 1 & 1), (m_rotAxis >> 2 & 1));
			
			m_rigidbody->setLinearFactor(axisMovement);
			m_rigidbody->setAngularFactor(axisRotation);
		}
		
		// Collision flags.
		{
			int flags = m_rigidbody->getCollisionFlags();
			flags |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;
			
			if(m_isTrigger)
			{
				flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
			}
			
			m_rigidbody->setCollisionFlags(flags);
		}
		
		CaffServ::PhysicsManager().addRigidbody(m_rigidbody.get());
	}
}


} // namespace
} // namespace
