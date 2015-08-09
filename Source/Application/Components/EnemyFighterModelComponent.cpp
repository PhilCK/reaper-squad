
#include <Application/Components/EnemyFighterModelComponent.hpp>
#include <Application/Common/ApplicationEvents.hpp>

#include <Caffeine/Entity/Entity.hpp>
#include <Caffeine/Components/PhysicsComponent.hpp>
#include <Caffeine/Components/TransformComponent.hpp>

#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Systems/ModelManager.hpp>
#include <Caffeine/Application/Model.hpp>
#include <Caffeine/Systems/EventManager.hpp>

namespace App {


EnemyFighterModelComponent::EnemyFighterModelComponent()
: CaffEnt::Component()
, m_health(10.0f)
{
}


EnemyFighterModelComponent::~EnemyFighterModelComponent()
{
}


void EnemyFighterModelComponent::onAttach()
{
	// -- Phys
	
	CaffComp::PhysicsComponent *phys = getOwner()->getRigidBody();
		
	phys->setMass(9);

	const CaffSys::ModelData &collisonMesh = CaffServ::ModelManager().getModelData("SpitfireCollision");
	CaffApp::Mesh &mesh = collisonMesh.model->getMesh(0);
	
	btTriangleMesh *bulletMesh = new btTriangleMesh();

	// TODO: EEEEK I had to go into blender to get the number of tri's
	for(uint16_t i = 0; i < 52; ++i)
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
	
	//btConvexTriangleMeshShape* shape = new btConvexTriangleMeshShape(bulletMesh);
	//phys->setCollisionShape(shape);
	
	//std::unique_ptr<CaffComp::Collider> collider(new CaffComp::BoxCollider(1.f, 1.f, 2.f));
	//phys->setColliderShape(std::move(collider));
}


void EnemyFighterModelComponent::onDisattach()
{
}


void EnemyFighterModelComponent::onThink(const float dt)
{
}


void EnemyFighterModelComponent::onCollision(const Caffeine::Entity::ColliderDetails &collider)
{
	if(getOwner()->isActive())
	{
		if(collider.collidedWith)
		{
			if(collider.collidedWith->getName() == "Bullet")
			{
				m_health -= 2;
				
				const glm::vec3 pos = getOwner()->getTransform()->getPosition();
				
				if(m_health < 0)
				{
					CaffSys::Event evt1 = SetupPlaneDestroyed(pos, getOwner());
					CaffServ::EventManager().sendInstantEvent(&evt1);
					
					CaffSys::Event evt2 = SetupEnemyFighterDestroyed(pos, getOwner());
					CaffServ::EventManager().sendInstantEvent(&evt2);
					
					getOwner()->setActive(false);
				}
				else
				{
					CaffSys::Event evt1 = SetupPlaneDamage(pos, getOwner());
					CaffServ::EventManager().sendInstantEvent(&evt1);
					
					CaffSys::Event evt2 = SetupEnemyFighterDamaged(pos, getOwner());
					CaffServ::EventManager().sendInstantEvent(&evt2);
				}
				
			}
		}
	}
}


COMPONENT_FACTORY_SOURCE(EnemyFighterModelComponent)

} // namespace
