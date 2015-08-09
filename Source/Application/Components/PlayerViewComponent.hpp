
/*
	PlayerViewComponent.hpp
	-----------------------
	
	Project: Reaper Squad
	
	Copyright of 'null-entity games'
	www.null-entity.com
*/

#ifndef APPLICATION_PLAYER_VIEW_COMPONENT_INCLUDED
#define APPLICATION_PLAYER_VIEW_COMPONENT_INCLUDED

#include <Caffeine/Components/RenderableComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class PlayerViewComponent : public Caffeine::Components::RenderableComponent
{
public:

	//COMPONENT_RTTI(PlayerView)

	explicit			PlayerViewComponent();
						~PlayerViewComponent();

// private:

	void				render() override;
	void				onAttach() override;
	void				onDisattach() override;
	
	void				onEarlyThink(const float dt) override;
	void				onThink(const float dt) override;
	void				onLateThink(const float dt) override;

}; // class

COMPONENT_FACTORY_INTERFACE(PlayerViewComponent)

} // namespace

#endif
