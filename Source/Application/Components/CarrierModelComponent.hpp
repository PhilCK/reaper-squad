
#ifndef APPLICATION_CARRIER_MODEL_COMPONENT_INCLUDED
#define APPLICATION_CARRIER_MODEL_COMPONENT_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

#include <glm/glm.hpp>

namespace App {

class CarrierModelComponent : public CaffEnt::Component
{
public:

	COMPONENT_RTTI(CarrierModel)
	
	explicit				CarrierModelComponent();
							~CarrierModelComponent();

	// Component Hooks //
	void					onStart() override;
	void					onEnd() override;
	
	void					loadParams(const std::string &params) override;
	void					onAttach() override;
	void					onDisattach() override;
	void					onCollision(const CaffEnt::ColliderDetails &collider) override;
	void					onThink(const float dt) override;
	
	glm::vec3				getSpawnPoint() const;
	glm::vec3				getSpawnRotation() const;

private:

	glm::vec3				m_spawnPoint;
	glm::vec3				m_spawnRotation;

}; // class

COMPONENT_FACTORY_INTERFACE(CarrierModelComponent)

} // namespace

#endif // include guard
