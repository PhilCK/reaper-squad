
#ifndef APPLICATION_CARRIER_VIEW_COMPONENT_INCLUDED
#define APPLICATION_CARRIER_VIEW_COMPONENT_INCLUDED

#include <Caffeine/Components/RenderableComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class CarrierViewComponent : public CaffComp::RenderableComponent
{
public:

	//COMPONENT_RTTI(CarrierView)

	explicit			CarrierViewComponent();
						~CarrierViewComponent();

// private:

	void				render() override;
	void				onAttach() override;
	void				onDisattach() override;
	
	void				onEarlyThink(const float dt) override;
	void				onThink(const float dt) override;
	void				onLateThink(const float dt) override;

}; // class

COMPONENT_FACTORY_INTERFACE(CarrierViewComponent)

} // namespace

#endif // include guard
