
#ifndef APPLICATION_INPUT_CONTROLLER_INCLUDED
#define APPLICATION_INPUT_CONTROLLER_INLCUDED

#include <Caffeine/Components/ControllerComponent.hpp>
#include <Caffeine/Systems/EntityFactory.hpp>
#include <Caffeine/Application/Input.hpp>
#include <array>

namespace App {

class InputControllerComponent : public CaffComp::ControllerComponent
{
public:

	explicit				InputControllerComponent();
							~InputControllerComponent();
							
				
	#pragma mark - Component Gubbins
	
	void					onAttach() override;
	void					onDisattach() override;
	void					onEarlyThink(const float dt) override;
	void					eventController(const CaffSys::Event *eventData) override;
	
	
	# pragma mark - Plane
		
	void					setPitchUpKey(const CaffApp::KeyID &key);
	void					setPitchDownKey(const CaffApp::KeyID &key);
	void					setRollLeftKey(const CaffApp::KeyID &key);
	void					setRollRightKey(const CaffApp::KeyID &key);
	void					setYawLeftKey(const CaffApp::KeyID &key);
	void					setYawRightKey(const CaffApp::KeyID &key);
	void					setShootingKey(const CaffApp::KeyID &key);
	void					setBoostKey(const CaffApp::KeyID &key);
	void					setSlowKey(const CaffApp::KeyID &key);


	#pragma mark - Navigation
	
	void					setNavigationUp(const CaffApp::KeyID &key);
	void					setNavigationDown(const CaffApp::KeyID &key);
	void					setNavigationLeft(const CaffApp::KeyID &key);
	void					setNavigationRight(const CaffApp::KeyID &key);
	void					setNavigationEnter(const CaffApp::KeyID &key);
	
	
	#pragma mark - Misc
	
	void					setCameraCycleKey(const CaffApp::KeyID &key);

	
	
private:

	bool					m_hasPlayerControl;

	enum class InputContext
	{
		NAVIGATION,
		PLAYER_MOVEMENT,
		
		SIZE,
	};

	InputContext			m_currentInputContext;


	enum InputKey
	{
		// Player Movement keys
		PITCH_UP_KEY,
		PITCH_DOWN_KEY,
		ROLL_LEFT_KEY,
		ROLL_RIGHT_KEY,
		YAW_LEFT_KEY,
		YAW_RIGHT_KEY,
		SHOOT_KEY,
		BOOST_KEY,
		SLOW_KEY,
		CAMERA_CYCLE_KEY,
		
		// Menu navigation keys
		NAV_UP_KEY,
		NAV_DOWN_KEY,
		NAV_LEFT_KEY,
		NAV_RIGHT_KEY,
		NAV_ENTER_KEY,
		
		SIZE,
	};

	std::array<CaffApp::KeyID, InputKey::SIZE>	m_boundKeys;

	enum class Camera
	{
		CHASE_CAM,
		FREE_CAM,
	};
	
	Camera					m_selectedCam;

}; // class


COMPONENT_FACTORY_INTERFACE(InputControllerComponent)

} // namespace

#endif // include guard
