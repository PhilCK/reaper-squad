
#ifndef APPLICATION_ENEMY_FIGHTER_COMPONENT_INCLUDED
#define APPLICATION_ENEMY_FIGHTER_COMPONENT_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class EnemyFighterModelComponent : public CaffEnt::Component
{
public:

	COMPONENT_RTTI(EnemyFighterModel)
	
	explicit			EnemyFighterModelComponent();
						~EnemyFighterModelComponent();
	
	void				onAttach() override;
	void				onDisattach() override;
	void				onThink(const float dt) override;
	
	void				onCollision(const CaffEnt::ColliderDetails &collider);
	
private:

	float				m_health;

}; // class

COMPONENT_FACTORY_INTERFACE(EnemyFighterModelComponent)

} // namespace

#endif // include guard
