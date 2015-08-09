
#ifndef CAFFEINE_SYSTEMS_PHYSICS_MANAGER_INCLUDED
#define CAFFEINE_SYSTEMS_PHYSICS_MANAGER_INCLUDED

#include <Caffeine/Platform.hpp>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <memory>

namespace Caffeine {
namespace Systems {

class PhysicsManager
{

public:

	explicit			PhysicsManager();
						~PhysicsManager();
				
				
	void				setGravity(const float* gravityVec);
	glm::vec3			getGravity() const;
	
	void				updateWorld(const float dt);
	void				renderDebug() const;
	
	
	
// TODO: Decide how this should be.
//private: Make private? only phys component can add?

	void				addRigidbody(btRigidBody *rigidbody);
	void				removeRigidbody(btRigidBody *rigidbody);
	
	btDiscreteDynamicsWorld& getWorld() const;
	// Remove??
	
private:

	struct Impl;
	std::unique_ptr<Impl> m_pImpl;
		

}; // class


} // namespace
} // namespace

#endif // include guard
