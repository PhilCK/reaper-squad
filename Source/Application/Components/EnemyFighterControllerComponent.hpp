
#ifndef APPLICATION_ENEMY_FIGHTER_CONTROLLER_INCLUDED
#define APPLICATION_ENEMY_FIGHTER_CONTROLLER_INCLUDED

#include <Caffeine/Components/ControllerComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class EnemyFighterControllerComponent : public CaffComp::ControllerComponent
{
public:

	explicit				EnemyFighterControllerComponent();
							~EnemyFighterControllerComponent();
				
	void					onAttach() override;
	void					onDisattach() override;
	void					onThink(const float dt) override;
	
	void					eventController(const CaffSys::Event *eventData) override;

}; // class

COMPONENT_FACTORY_INTERFACE(EnemyFighterControllerComponent)

} // namespace

#endif // include guard
