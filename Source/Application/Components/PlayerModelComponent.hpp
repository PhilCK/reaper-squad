
#ifndef APPLICATION_PLAYER_MODEL_INCLUDED
#define APPLICATION_PLAYER_MODEL_INCLUDED

#include <Caffeine/Entity/Component.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <glm/glm.hpp>

namespace App {

class PlayerModelComponent : public CaffEnt::Component
{
public:

	COMPONENT_RTTI(PlayerModel)
	
	explicit				PlayerModelComponent();
							~PlayerModelComponent();
	
	void					onStart() override;
	void					onEnd() override;
	
	void					onAttach() override;
	void					onDisattach() override;
	
	void					onCollision(const CaffEnt::ColliderDetails &collider) override;
	
	void					onThink(const float dt) override;

	inline float			getThrust() const   { return m_thrust;   }
	inline float			getRudder() const   { return m_rudder;   }
	inline float			getElevator() const { return m_elevator; }
	inline float			getAileron() const  { return m_aileron;  }

private:

	friend class PlayerControllerComponent;

	void					applyThrust(const float thrust);
	void					applyRudder(const float rudder);
	void					applyElevator(const float elevator);
	void					applyAileron(const float aileron);
	
	// Forces
	float					m_thrust,   m_thrustAccel,   m_maxThrust,   m_minThrust,	m_applyThust,	m_thrustFalloff;
	float					m_rudder,   m_rudderAccel,   m_maxRudder,   m_minRudder,	m_applyRudder, m_rudderFalloff;
	float					m_elevator, m_elevatorAccel, m_maxElevator, m_minElevator,	m_applyElevator, m_elevatorFalloff;
	float					m_aileron,  m_aileronAccel,  m_maxAileron,  m_minAileron,	m_applyAileron, m_aileronFalloff;
	
	// Flight dynamics
	float					m_lift,		m_maxLift;
	

}; // class


COMPONENT_FACTORY_INTERFACE(PlayerModelComponent)
} // namespace

#endif // include guard
