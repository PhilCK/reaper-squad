
#ifndef APPLICATION_CARRIER_CONTROLLER_INCLUDED
#define APPLICATION_CARRIER_CONTROLLER_INCLUDED

#include <Caffeine/Components/ControllerComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>

namespace App {

class CarrierControllerComponent : public CaffComp::ControllerComponent
{
public:
	
	explicit			CarrierControllerComponent();
						~CarrierControllerComponent();
	
	void				onStart() override;
	void				onAttach() override;
	void				onDisattach() override;
	void				onThink(const float dt) override;
	
	void				eventController(const CaffSys::Event *eventData) override;
	
private:

	bool				m_hasPlayer;
	bool				m_launchPlayer;
	float				m_controlTimer;
	float				m_maxTimer;
	

}; // class

COMPONENT_FACTORY_INTERFACE(CarrierControllerComponent)

} // namespace

#endif // include guard
