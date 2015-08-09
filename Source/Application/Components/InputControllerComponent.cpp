
#include <Caffeine/Services/Services.hpp>
#include <Caffeine/Application/Input.hpp>
#include <Caffeine/Systems/EventManager.hpp>
#include <Application/Components/InputControllerComponent.hpp>
#include <Application/Common/ApplicationEvents.hpp>


namespace {

	float GetAircraftAxis(const CaffApp::Input &input, const CaffApp::KeyID &key, const float direction)
	{
		if((unsigned int)key >= (unsigned int)CaffApp::KeyID::MS_XAXIS)
		{
			return input.getAxis(key);
		}
	
		return input.getAxis(key) * direction;
	}

}


namespace App {

InputControllerComponent::InputControllerComponent()
: CaffComp::ControllerComponent()
, m_hasPlayerControl(false)
, m_currentInputContext(InputContext::PLAYER_MOVEMENT)
, m_selectedCam(Camera::CHASE_CAM)
{
	// Default keys
	m_boundKeys[PITCH_UP_KEY]		= CaffApp::KeyID::KB_W;
	m_boundKeys[PITCH_DOWN_KEY]		= CaffApp::KeyID::KB_S;
	m_boundKeys[ROLL_LEFT_KEY]		= CaffApp::KeyID::MS_XAXIS;
	m_boundKeys[ROLL_RIGHT_KEY]		= CaffApp::KeyID::MS_XAXIS;
	m_boundKeys[YAW_LEFT_KEY]		= CaffApp::KeyID::KB_A;
	m_boundKeys[YAW_RIGHT_KEY]		= CaffApp::KeyID::KB_D;
	m_boundKeys[SHOOT_KEY]			= CaffApp::KeyID::KB_SPACE;
	m_boundKeys[BOOST_KEY]			= CaffApp::KeyID::KB_LSHIFT;
	m_boundKeys[SLOW_KEY]			= CaffApp::KeyID::KB_LCTRL;
	m_boundKeys[CAMERA_CYCLE_KEY]	= CaffApp::KeyID::KB_TAB;
	m_boundKeys[NAV_UP_KEY]			= CaffApp::KeyID::KB_UP;
	m_boundKeys[NAV_DOWN_KEY]		= CaffApp::KeyID::KB_DOWN;
	m_boundKeys[NAV_LEFT_KEY]		= CaffApp::KeyID::KB_LEFT;
	m_boundKeys[NAV_RIGHT_KEY]		= CaffApp::KeyID::KB_RIGHT;
	m_boundKeys[NAV_ENTER_KEY]		= CaffApp::KeyID::KB_ENTER;
}


InputControllerComponent::~InputControllerComponent()
{
}


#pragma mark - Component Gubbins

void InputControllerComponent::onAttach()
{
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_PLANE_SUSPENDED_MSG);
	CaffServ::EventManager().addController(this, (unsigned int)EventID::INPUT_PLANE_RESUME_MSG);
}


void InputControllerComponent::onDisattach()
{
	CaffServ::EventManager().removeController(this, (unsigned int)EventID::INPUT_PLANE_SUSPENDED_MSG);
	CaffServ::EventManager().removeController(this, (unsigned int)EventID::INPUT_PLANE_RESUME_MSG);
}


void InputControllerComponent::onEarlyThink(const float dt)
{
	// TODO: Input should be 100% event driven to ensure it happens before any logic.
	const CaffApp::Input		&input  = CaffServ::InputManager();
		  CaffSys::EventManager &evtMgr = CaffServ::EventManager();

	// Player movment control.
	if((m_currentInputContext == InputContext::PLAYER_MOVEMENT) && m_hasPlayerControl)
	{
		if(input.isDown(m_boundKeys[PITCH_UP_KEY]))
		{
			const float axis = GetAircraftAxis(input, m_boundKeys[PITCH_UP_KEY], +1.f);
						
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_PITCH_MSG, axis);
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[PITCH_DOWN_KEY]))
		{
			const float axis = GetAircraftAxis(input, m_boundKeys[PITCH_DOWN_KEY], -1.f);
		
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_PITCH_MSG, axis);
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[ROLL_LEFT_KEY]))
		{
			const float axis = GetAircraftAxis(input, m_boundKeys[ROLL_LEFT_KEY], +1.f);
		
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_ROLL_MSG, axis);
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[ROLL_RIGHT_KEY]))
		{
			const float axis = GetAircraftAxis(input, m_boundKeys[ROLL_RIGHT_KEY], -1.f);
		
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_ROLL_MSG, axis);
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[YAW_LEFT_KEY]))
		{
			const float axis = GetAircraftAxis(input, m_boundKeys[YAW_LEFT_KEY], +1.f);
		
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_YAW_MSG, axis);
			evtMgr.sendInstantEvent(&evt);
		}
	
		if(input.isDown(m_boundKeys[YAW_RIGHT_KEY]))
		{
			const float axis = GetAircraftAxis(input, m_boundKeys[YAW_RIGHT_KEY], -1.f);
		
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_YAW_MSG, axis);
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[SHOOT_KEY]))
		{		
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_SHOOT_MSG, input.getAxis(m_boundKeys[SHOOT_KEY]));
			evtMgr.sendInstantEvent(&evt);
		}

		if(input.isDown(m_boundKeys[BOOST_KEY]))
		{
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_BOOST_MSG, input.getAxis(m_boundKeys[BOOST_KEY]));
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[SLOW_KEY]))
		{
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_PLANE_SLOW_MSG, input.getAxis(m_boundKeys[SLOW_KEY]));
			evtMgr.sendInstantEvent(&evt);
		}
	}
	
	// Currently outside player movement to allow for debug camera to be used.
	if(input.isDownOnFrame(m_boundKeys[CAMERA_CYCLE_KEY]))
	{
		const std::string	cam = m_selectedCam == Camera::CHASE_CAM ? "FreeCam" : "ChaseCam";
		m_selectedCam = m_selectedCam == Camera::CHASE_CAM ? Camera::FREE_CAM : Camera::CHASE_CAM;
		
		CaffSys::Event		evt = SetupCameraCycle(cam.c_str());
		
		evtMgr.sendInstantEvent(&evt);
	}
	
	// Menu Navigation.
	else if(m_currentInputContext == InputContext::NAVIGATION)
	{
		if(input.isDown(m_boundKeys[NAV_UP_KEY]))
		{
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_UP_MSG, input.getAxis(m_boundKeys[NAV_UP_KEY]));
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[NAV_DOWN_KEY]))
		{
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_DOWN_MSG, input.getAxis(m_boundKeys[NAV_DOWN_KEY]));
			evtMgr.sendInstantEvent(&evt);
		}

		if(input.isDown(m_boundKeys[NAV_LEFT_KEY]))
		{
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_LEFT_MSG, input.getAxis(m_boundKeys[NAV_LEFT_KEY]));
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[NAV_RIGHT_KEY]))
		{
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_RIGHT_MSG, input.getAxis(m_boundKeys[NAV_RIGHT_KEY]));
			evtMgr.sendInstantEvent(&evt);
		}
		
		if(input.isDown(m_boundKeys[NAV_ENTER_KEY]))
		{
			CaffSys::Event evt = SetupInputEvent(EventID::INPUT_ENTER_MSG, input.getAxis(m_boundKeys[NAV_ENTER_KEY]));
			evtMgr.sendInstantEvent(&evt);
		}
	}
}


void InputControllerComponent::eventController(const CaffSys::Event *eventData)
{
	if(IsAppEvent(eventData, EventID::INPUT_PLANE_SUSPENDED_MSG))
	{
		m_hasPlayerControl = false;
		m_currentInputContext = InputContext::NAVIGATION;
	}
	else if(IsAppEvent(eventData, EventID::INPUT_PLANE_RESUME_MSG))
	{
		m_hasPlayerControl = true;
		m_currentInputContext = InputContext::PLAYER_MOVEMENT;
	}
}


# pragma mark - Plane
	
void InputControllerComponent::setPitchUpKey(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setPitchDownKey(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setRollLeftKey(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setRollRightKey(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setYawLeftKey(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setYawRightKey(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setShootingKey(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setBoostKey(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setSlowKey(const CaffApp::KeyID &key)
{}


#pragma mark - Navigation

void InputControllerComponent::setNavigationUp(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setNavigationDown(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setNavigationLeft(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setNavigationRight(const CaffApp::KeyID &key)
{}


void InputControllerComponent::setNavigationEnter(const CaffApp::KeyID &key)
{}



#pragma mark - Misc

void InputControllerComponent::setCameraCycleKey(const CaffApp::KeyID &key)
{}


COMPONENT_FACTORY_SOURCE(InputControllerComponent)

} // namespace
