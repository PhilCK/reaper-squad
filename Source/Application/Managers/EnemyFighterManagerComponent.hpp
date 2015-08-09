
#ifndef APPLICATION_ENEMY_FIGHTER_MANAGER_COMPONENT_INCLUDED
#define APPLICATION_ENEMY_FIGHTER_MANAGER_COMPONENT_INCLUDED

#include <Caffeine/Components/ControllerComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class EnemyFighterManagerComponent : public CaffComp::ControllerComponent
{
public:

	COMPONENT_RTTI(EnemyFighterManager)

	explicit			EnemyFighterManagerComponent();
						~EnemyFighterManagerComponent();
				
	void				onAttach() override;
	void				onStart() override;
	void				onDisattach() override;
	
	void				eventController(const CaffSys::Event* eventData) override;

}; // class

COMPONENT_FACTORY_INTERFACE(EnemyFighterManagerComponent)

} // namespace

#endif // include guard
