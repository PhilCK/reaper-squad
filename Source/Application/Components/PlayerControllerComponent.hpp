
#ifndef APPLICATION_PLAYER_CONTROLLER_INCLUDED
#define APPLICATION_PLAYER_CONTROLLER_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Systems/Event.hpp>
#include <Application/Components/PlayerModelComponent.hpp>

namespace App {


class PlayerControllerComponent : public Caffeine::Entity::Component, public Caffeine::Systems::Eventable
{
public:

	COMPONENT_RTTI(PlayerController)

	explicit				PlayerControllerComponent();
							~PlayerControllerComponent();
				
	void					onStart() override;
	void					onEnd() override;

	void					onAttach() override;
	void					onDisattach() override;
	
	void					onEarlyThink(const float dt) override;
	void					onThink(const float dt) override;
	void					onLateThink(const float dt) override;
	
	void					eventController(const Caffeine::Systems::Event *eventData);
	
private:
	
	float					m_rotation;
	float					m_inputRot;
	
	PlayerModelComponent*	m_playerModel;
	bool					m_active;
	float					m_coolDown;
	float					m_currentCoolDown;
	
}; // class


COMPONENT_FACTORY_INTERFACE(PlayerControllerComponent)

} // namespace



#endif // include guard
