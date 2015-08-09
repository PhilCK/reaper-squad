
#ifndef APPLICATION_LAYER_VIEW_COMPONENT_INCLUDED
#define APPLICATION_LAYER_VIEW_COMPONENT_INCLUDED

#include <Caffeine/Components/RenderableComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class LevelViewComponent : public Caffeine::Components::RenderableComponent
{
public:

	//COMPONENT_RTTI(LevelView)

	explicit			LevelViewComponent();
						~LevelViewComponent();

// private:

	void				render() override;
	void				onAttach() override;
	void				onDisattach() override;
	
	void				onEarlyThink(const float dt) override;
	void				onThink(const float dt) override;
	void				onLateThink(const float dt) override;

}; // class

COMPONENT_FACTORY_INTERFACE(LevelViewComponent)

} // namespace

#endif // include guard
