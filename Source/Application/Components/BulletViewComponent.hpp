
#ifndef APPLICATION_BULLET_VIEW_CONTROLLER_INCLUDED
#define APPLICATION_BULLET_VIEW_CONTROLLER_INCLUDED

#include <Caffeine/Components/RenderableComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class BulletViewComponent : public CaffComp::RenderableComponent
{
public:

	explicit				BulletViewComponent();
							~BulletViewComponent();
				
	void					onAttach() override;
	void					onDisattach() override;							
				
	void					render() override;
	
	void					onThink(const float dt) override;
		
}; // class


COMPONENT_FACTORY_INTERFACE(BulletViewComponent)


} // namespace

#endif // include guard
