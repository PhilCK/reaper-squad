
#ifndef APPLICATION_ENEMY_FIGHTER_VIEW_COMPONENT_INCLUDED
#define APPLICATION_ENEMY_FIGHTER_VIEW_COMPONENT_INCLUDED

#include <Caffeine/Components/RenderableComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class EnemyFighterViewComponent : public CaffComp::RenderableComponent
{
public:

	explicit				EnemyFighterViewComponent();
							~EnemyFighterViewComponent();
				
	void					onAttach() override;
	void					onDisattach() override;							
				
	void					render() override;
	
	void					onThink(const float dt) override;

}; // class

COMPONENT_FACTORY_INTERFACE(EnemyFighterViewComponent)

} // namespace

#endif // include guard
